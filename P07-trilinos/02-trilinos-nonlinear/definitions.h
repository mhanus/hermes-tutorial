#include "hermes2d.h"

using namespace Hermes;
using namespace Hermes::Hermes2D;
using namespace Hermes::Hermes2D::Views;

class CustomRightHandSide: public Hermes2DFunction<double>
{
public:
  CustomRightHandSide() : Hermes2DFunction<double>() {};

  virtual double value(double x, double y) const;

  virtual Ord ord(Ord x, Ord y) const;

  template<typename Real>
  Real u(Real x, Real y) const;
  template<typename Real>
  Real dudx(Real x, Real y) const;
  template<typename Real>
  Real dudy(Real x, Real y) const;

  template<typename Real>
  Real dudxx(Real x, Real y) const;
  template<typename Real>
  Real dudyy(Real x, Real y) const;
  template<typename Real>
  Real dudxy(Real x, Real y) const;

  template<typename Real>
  Real k(Real x, Real y) const;
  template<typename Real>
  Real kx(Real x, Real y) const;
  template<typename Real>
  Real ky(Real x, Real y) const;
};

/* Exact solution */

class CustomExactSolution : public ExactSolutionScalar<double>
{
public:
  CustomExactSolution(Mesh* mesh)
             : ExactSolutionScalar<double>(mesh) {};

  double value(double x, double y) const;

  virtual void derivatives (double x, double y, double& dx, double& dy) const;

  virtual Ord ord(Ord x, Ord y) const;
};

/* Initial solution */

class CustomInitialSolution : public ExactSolutionScalar<double>
{
public:
  CustomInitialSolution(Mesh* mesh)
               : ExactSolutionScalar<double>(mesh) {};

  double value(double x, double y) const;

  virtual void derivatives (double x, double y, double& dx, double& dy) const;

  virtual Ord ord(Ord x, Ord y) const;
};

/* Weak form */
class CustomWeakForm : public WeakForm<double>
{
public:
  CustomWeakForm(bool JFNK = false, bool precondition_jacobian = false, bool precondition_jacobian_approx = false);

  ~CustomWeakForm() {}

private:
  class JacobianFormVol : public MatrixFormVol<double>
  {
  public:
    JacobianFormVol(int i, int j) : MatrixFormVol<double>(i, j, HERMES_ANY, HERMES_SYM) {};

    virtual double value(int n, double *wt, Func<double> *u_ext[], Func<double> *u, 
                 Func<double> *v, Geom<double> *e, ExtData<double> *ext) const;

    virtual Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, 
            Geom<Ord> *e, ExtData<Ord> *ext) const;
  };

  class ResidualFormVol : public VectorFormVol<double>
  {
  public:
    ResidualFormVol(int i, CustomRightHandSide* rhs) : VectorFormVol<double>(i), rhs(rhs) {};

    virtual double value(int n, double *wt, Func<double> *u_ext[], Func<double> *v, 
                         Geom<double> *e, ExtData<double> *ext) const;

    virtual Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, 
                    Geom<Ord> *e, ExtData<Ord> *ext) const;

  private:
    // Problem parameters.
    CustomRightHandSide* rhs;
  };

  class PrecondFormVol : public MatrixFormVol<double>
  {
  public:
    PrecondFormVol(int i, int j) : MatrixFormVol<double>(i, j, HERMES_ANY, HERMES_SYM) {};

    virtual double value(int n, double *wt, Func<double> *u_ext[], Func<double> *u, 
                 Func<double> *v, Geom<double> *e, ExtData<double> *ext) const;

    virtual Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, 
            Geom<Ord> *e, ExtData<Ord> *ext) const;
  };
};

