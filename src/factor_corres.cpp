#include <Rcpp.h>
using namespace Rcpp;

//' Perform the iterative PAF procedure
//'
//' Function called from within PAF so usually no call to this is needed by the user.
//' Provides a C++ implementation of the PAF procedure
//'
//' @param x numeric matrix. The initial communality estimates.
//' @param y numeric matrix. The convergence criterion to use.
//' @param thresh numeric. The threshold to classify a pattern coefficient as substantial.
//' @export
// [[Rcpp::export]]
Rcpp::List factor_corres(NumericMatrix x,
                         NumericMatrix y,
                         double thresh = 0.3) {

  IntegerVector x_corres;
  IntegerVector y_corres;
  int diff_corres;

  StringVector x_corres_cross;
  StringVector y_corres_cross;
  int diff_corres_cross;

  Rcpp::CharacterVector x_pos;
  Rcpp::CharacterVector y_pos;
  Rcpp::LogicalVector x_log;
  Rcpp::LogicalVector y_log;
  int temp;

  for (int i = 0; i<x.nrow(); i++) {

    x_pos.erase(0, x_pos.size());
    y_pos.erase(0, y_pos.size());

    x_log = abs(x.row(i)) >= thresh;
    y_log = abs(y.row(i)) >= thresh;

    if (sum(x_log) > 0 && sum(y_log) > 0) {

      x_corres.push_back(which_max(abs(x.row(i))));
      y_corres.push_back(which_max(abs(y.row(i))));

      for (int jj = 0; jj<x_log.size(); jj++) {
        if (x_log[jj]) {
          temp = (jj + 1);
          x_pos.push_back((char)temp);
        }
        if (y_log[jj]) {
          temp = (jj + 1);
          y_pos.push_back((char)temp);
        }
      }
    } else if (sum(x_log) > 0) {

      x_corres.push_back(which_max(abs(x.row(i))));
      y_corres.push_back(0);

      for (int jj = 0; jj<x_log.size(); jj++) {
        if (x_log[jj]) {
          temp = (i + 1);
          x_pos.push_back((char)temp);
        }
      }
      y_pos.push_back('0');
    } else if (sum(y_log) > 0) {

      x_corres.push_back(0);
      y_corres.push_back(which_max(abs(y.row(i))));

      for (int jj = 0; jj<y_log.size(); jj++) {
        if (x_log[jj]) {
          temp = (i + 1);
          y_pos.push_back((char)temp);
        }
      }
      x_pos.push_back('0');
    } else {
      x_corres.push_back(0);
      y_corres.push_back(0);
      x_pos.push_back('0');
      y_pos.push_back('0');
    }

    x_corres_cross.push_back(collapse(x_pos));
    y_corres_cross.push_back(collapse(y_pos));
  }

  diff_corres = sum(x_corres != y_corres);
  diff_corres_cross = sum(x_corres_cross != y_corres_cross);

  return Rcpp::List::create(Rcpp::Named("x_corres") = x_corres,
                            Rcpp::Named("y_corres") = y_corres,
                            Rcpp::Named("diff_corres") = diff_corres,
                            Rcpp::Named("x_corres_cross") = x_corres_cross,
                            Rcpp::Named("y_corres_cross") = y_corres_cross,
                            Rcpp::Named("diff_corres_cross") = diff_corres_cross);
}
