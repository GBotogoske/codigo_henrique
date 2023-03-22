#include <TH1F.h>
#include <TMath.h>

void fill_histogram() 
{
  TH1F *hist = new TH1F("hist", "Exponential Function Histogram", 1000, 0, 10);

  const double A = 1.0;
  const double b = 0.5;

  for (int i = 1; i <= hist->GetNbinsX(); ++i) {
    double x = hist->GetBinCenter(i);
    double y = A*TMath::Exp(-b*x);
    hist->Fill(x, y);
  }

 
}

