#include "utils.h"


int main(int argc, const char *argv[]) {
    printf("afmCond starting\n\n");

    /////////////////// Default hamiltonian parameters //////////////////

    double ETA=0.1;
    double t=1.0;
    double tp=-0.3;
    double tpp=0.2;
    double beta=100.0;

    int nK = 401;

    int nMu = 20;
    double muMin=-4.0;
    double muMax=4.0;

    int nOmega = 11;
    double amplitudeCutoff = 0.005;

    ////////////////// Read parameters from file //////////////////

    FILE * file = fopen("./examples/model.dat", "rt");
    if (file == NULL) {
        printf("File %s not found", "model.dat");
        exit(1);
    }
    printf("Reading parameters from model.dat\n\n");

    readDouble(file, "tpp",  &tpp);
    readDouble(file, "tp",   &tp);
    readDouble(file, "t",    &t);
    readDouble(file, "ETA",  &ETA);
    readDouble(file, "beta", &beta);
    readDouble(file, "muMin",&muMin);
    readDouble(file, "muMax",&muMax);

    readDouble(file, "amplitudeCutoff", &amplitudeCutoff);

    readInt(file, "nK",     &nK);
    readInt(file, "nMu",    &nMu);
    readInt(file, "nOmega", &nOmega);

    fclose(file);

    ////////////////// Out data file initialization //////////////////

    char * headers[] = {
        "#mu", "density",
        "sigma_xx", "sigma_xy",
        "alpha_xx", "alpha_xy",
        "beta_xx", "beta_xy"
    };

    FILE * fileOut = fopen("./examples/conductivities.dat", "w");
    writeHeader(fileOut, headers);

    //////// precalculate omega vector Fermi Dirac derivative vector ////////

    double energyCutoff = 2.*2.*acosh(0.25*sqrt(beta/amplitudeCutoff)) /beta;
    // we put an additionnal factor of 2. for the derivative cutoff

    double omega[nOmega];
    double dfermiDirac_dw[nOmega];

    int n=0; for(n=0; n<nOmega; n++)
    {
        omega[n]= -energyCutoff + 2.*energyCutoff*n/(nOmega-1);
        double expBw = exp(beta*omega[n]);
        dfermiDirac_dw[n]= -beta*expBw/((expBw+1.)*(expBw+1.));
    }

    ////////////////// precalculate trigo and k array //////////////////

    double sink[nK]; double sin2k[nK];
    double cosk[nK]; double cos2k[nK];

    int i=0; for(i=0; i<nK; i++)
    {
        double k = M_PI*(-1.0 + i*2.0/nK);

        sink[i] = sin(k); sin2k[i] = sin(2.*k);
        cosk[i] = cos(k); cos2k[i] = cos(2.*k);
    }

    ////////////////// main loop over mu, kx and ky //////////////////

    int m=0; for(m=0; m<nMu; m++)
    {
        double mu = muMin + m*(muMax-muMin)/(nMu-1);

        double sigma_xx = 0., sigma_xy = 0.;
        double alpha_xx = 0., alpha_xy = 0.;
        double beta_xx =  0., beta_xy = 0.;
        double density =  0.;

        int i; for(i=0; i<nK; i++) // kx = k[i]
        {
            int j=0; for(j=0; j<nK; j++) // ky = k[j]
            {
                ////////// dispersion relation (and its derivatives) /////////

                double epsilon_k           = -2.*t*(cosk[i] + cosk[j]) - 4.*tp*cosk[i]*cosk[j] - 2.*tpp*(cos2k[i] + cos2k[j]) - mu;
                double depsilon_k_dkx      =  2.*t*sink[i]             + 4.*tp*sink[i]*cosk[j] + 4.*tpp*sin2k[i];
                double depsilon_k_dky      =  2.*t*sink[j]             + 4.*tp*cosk[i]*sink[j] + 4.*tpp*sin2k[j];
                double ddepsilon_k_dkx_dkx =  2.*t*cosk[i]             + 4.*tp*cosk[i]*cosk[j] + 8.*tpp*cos2k[i];
                double ddepsilon_k_dky_dky =  2.*t*cosk[j]             + 4.*tp*cosk[i]*cosk[j] + 8.*tpp*cos2k[j];
                double ddepsilon_k_dkx_dky =                           - 4.*tp*sink[i]*sink[j];

                //////// eigen values and their derivatives (vertices) ////////

                double E1_k           = epsilon_k;
                double dE1_k_dkx      = depsilon_k_dkx;
                double dE1_k_dky      = depsilon_k_dky;
                double ddE1_k_dkx_dkx = ddepsilon_k_dkx_dkx;
                double ddE1_k_dky_dky = ddepsilon_k_dky_dky;
                double ddE1_k_dkx_dky = ddepsilon_k_dkx_dky;

                double kernel_xx = dE1_k_dkx*dE1_k_dkx;
                double kernel_xy = -(2./3.)*(dE1_k_dkx*(dE1_k_dkx*ddE1_k_dky_dky - dE1_k_dky*ddE1_k_dkx_dky));


                for(n=0; n<nOmega; n++)
                {
                  double complex z = omega[n] + ETA * I;

                  double A1_k = -(1./M_PI)*cimag(1.0/ (z-E1_k) );

                  double frequencyKernel_xx = -dfermiDirac_dw[n]*kernel_xx*A1_k*A1_k;
                  double frequencyKernel_xy = -dfermiDirac_dw[n]*kernel_xy*A1_k*A1_k*A1_k;

                  sigma_xx += frequencyKernel_xx;
                  sigma_xy += frequencyKernel_xy;

                  alpha_xx += beta*omega[n] * frequencyKernel_xx;
                  alpha_xy += beta*omega[n] * frequencyKernel_xy;

                  double omega2 = beta*beta* omega[n] * omega[n];
                  beta_xx += omega2 * frequencyKernel_xx;
                  beta_xy += omega2 * frequencyKernel_xy;
                }

                density += 1.0/(1.0+exp(beta*E1_k));
          }
       }

        double f0 = 1.0/nK/nK;
        double f = (2.*energyCutoff) /(nOmega)*f0;

        printf("\n% 4.8f % 4.8f % 4.8e % 4.8e", mu, f0*density,  f*sigma_xx, f*sigma_xy);

        fprintf(fileOut,"% 4.8f % 4.8f  ", mu, f0*density);
        fprintf(fileOut,"% 4.8e % 4.8e  ", f*sigma_xx, f*sigma_xy);
        fprintf(fileOut,"% 4.8e % 4.8e  ", f*alpha_xx, f*alpha_xy);
        fprintf(fileOut,"% 4.8e % 4.8e", f*beta_xx, f*beta_xy);
        fprintf(fileOut,"\n");
    }

    fclose(fileOut);
    char * msg = "afmCond over!";
    printInfo(msg);

    return 0;
}
