/****************************************************************************\
  Copyright (c) Enrico Bertolazzi 2016
  All Rights Reserved.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the file license.txt for more details.
\****************************************************************************/

#include "Clothoid.hh"
#include "Triangle2D.hh"
#include "mex_utils.hh"

#define MEX_ERROR_MESSAGE \
"=====================================================================================\n" \
"ClothoidCurveMexWrapper:  Compute parameters of the G1 Hermite clothoid fitting\n" \
"\n" \
"USAGE:\n" \
"  - Constructors:\n" \
"    OBJ = ClothoidCurveMexWrapper( 'new' ) ;\n" \
"    OBJ = ClothoidCurveMexWrapper( 'new', x0, y0, theta0, k0, dk, L ) ;\n" \
"\n" \
"    On input:\n" \
"      x0, y0 = coordinate of initial point\n" \
"      theta0 = orientation (angle) of the clothoid at initial point" \
"      k0     = curvature of the clothoid at initial point\n" \
"      dk     = derivative of curvature respect to arclength\n" \
"      L      = length of the clothoid curve from initial to final point\n" \
"\n" \
"     On output:\n" \
"       OBJ   = pointer to the internal object\n" \
"n" \
"  - Destructor:\n" \
"    ClothoidCurveMexWrapper( 'delete', OBJ ) ;\n" \
"\n" \
"  - Build:\n" \
"    ClothoidCurveMexWrapper( 'build', OBJ, x0, y0, theta0, k0, dk, L ) ;\n" \
"    ClothoidCurveMexWrapper( 'build_G1', OBJ, x0, y0, theta0, x1, y1, theta1 ) ;\n" \
"    [ L_D, k_D, dk_D ] = ClothoidCurveMexWrapper( 'build_G1_D', OBJ, x0, y0, theta0, x1, y1, theta1 ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'build_forward', OBJ,x0,y0,theta0,k0,x1,y1 ) ;\n" \
"    ClothoidCurveMexWrapper( 'copy', OBJ, OBJ1 ) ;\n" \
"\n" \
"  - Eval:\n" \
"    [x,y,theta,kappa] = ClothoidCurveMexWrapper( 'evaluate', OBJ, ss ) ;\n" \
"    [x0,y0,theta0,k0,dk,smin,smax] = ClothoidCurveMexWrapper( 'getPars', OBJ ) ;\n" \
"\n" \
"    [x,y]         = ClothoidCurveMexWrapper( 'eval', OBJ, ss[, offs] ) ;\n" \
"    [x_D,y_D]     = ClothoidCurveMexWrapper( 'eval_D', OBJ, ss[, offs] ) ;\n" \
"    [x_DD,y_DD]   = ClothoidCurveMexWrapper( 'eval_DD', OBJ, ss[, offs] ) ;\n" \
"    [x_DDD,y_DDD] = ClothoidCurveMexWrapper( 'eval_DDD', OBJ, ss[, offs] ) ;\n" \
"\n" \
"  - Transform:\n" \
"    ClothoidCurveMexWrapper( 'trim', OBJ, smin, smax ) ;\n" \
"    ClothoidCurveMexWrapper( 'changeOrigin', OBJ, newX0, newY0 ) ;\n" \
"    ClothoidCurveMexWrapper( 'rotate', OBJ, angle, cx, cy ) ;\n" \
"    ClothoidCurveMexWrapper( 'translate', OBJ, tx, ty ) ;\n" \
"    ClothoidCurveMexWrapper( 'scale', OBJ, scaling ) ;\n" \
"    ClothoidCurveMexWrapper( 'reverse', OBJ ) ;\n" \
"    [xp, yp, xm, ym] = ClothoidCurveMexWrapper( 'infinity', OBJ ) ;\n" \
"  - Boundary:\n" \
"    pt  = ClothoidCurveMexWrapper( 'xyBegin', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'xBegin', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'yBegin', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'thetaBegin', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'kappaBegin', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'kappaEnd', OBJ ) ;\n" \
"    pt  = ClothoidCurveMexWrapper( 'xyEnd', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'xEnd', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'yEnd', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'thetaEnd', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'kappaEnd', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'kappa_D', OBJ ) ;\n" \
"    res = ClothoidCurveMexWrapper( 'length', OBJ ) ;\n" \
"\n" \
"  - Distance:\n" \
"    [X,Y,s,dst] = ClothoidCurveMexWrapper( 'closestPoint', OBJ, x, y ) ;\n" \
"    [dst,s]     = ClothoidCurveMexWrapper( 'distance', OBJ, x, y ) ;\n" \
"    [X,Y,s,dst] = ClothoidCurveMexWrapper( 'closestPointBySample', OBJ, x, y, ds ) ;\n" \
"    [dst,s]     = ClothoidCurveMexWrapper( 'distanceBySample', OBJ, x, y, ds ) ;\n" \
"    [s,t]       = ClothoidCurveMexWrapper( 'findST', OBJ, x, y ) ;\n" \
"\n" \
"  - Intersection:\n" \
"    [s1,s2] = ClothoidCurveMexWrapper( 'intersect_line', OBJ, OBJ2 ) ;%\n" \
"    [s1,s2] = ClothoidCurveMexWrapper( 'intersect_circle', OBJ, OBJ2 ) ;%\n" \
"    [s1,s2] = ClothoidCurveMexWrapper( 'intersect_clothoid', OBJ, OBJ2 ) ;%\n" \
"    [s1,s2] = ClothoidCurveMexWrapper( 'intersect_clothoid_list', OBJ, OBJ2 ) ;%\n" \
"\n" \
"  - Bounding Box:\n" \
"    TT = ClothoidCurveMexWrapper( 'bbox', OBJ, max_angle, max_size ) ;%\n" \
"    TT = ClothoidCurveMexWrapper( 'bbox', OBJ, max_angle, max_size, offs ) ;%\n" \
"\n" \
"=====================================================================================\n" \
"\n" \
"Autors: Enrico Bertolazzi^(1), Marco Frego^(2), Paolo Bevilacqua^(2)\n" \
"  (1) Department of Industrial Engineering\n" \
"  (2) Department of Information Engineering and Computer Science\n" \
"  University of Trento\n" \
"  enrico.bertolazzi@unitn.it\n" \
"  m.fregox@gmail.com\n" \
"  paolo.bevilacqua@unitn.it\n" \
"\n" \
"=====================================================================================\n"

namespace G2lib {

  using namespace std;

  static
  ClothoidCurve *
  DATA_NEW( mxArray * & mx_id ) {
    ClothoidCurve * ptr = new ClothoidCurve();
    mx_id = convertPtr2Mat<ClothoidCurve>(ptr);
    return ptr ;
  }

  static
  inline
  ClothoidCurve *
  DATA_GET( mxArray const * & mx_id ) {
    return convertMat2Ptr<ClothoidCurve>(mx_id);
  }

  static
  void
  DATA_DELETE( mxArray const * & mx_id ) {
    destroyObject<ClothoidCurve>(mx_id);
  }

  extern "C"
  void
  mexFunction( int nlhs, mxArray       *plhs[],
               int nrhs, mxArray const *prhs[] ) {


    // the first argument must be a string
    if ( nrhs == 0 ) {
      mexErrMsgTxt(MEX_ERROR_MESSAGE) ;
      return ;
    }

    try {

      MEX_ASSERT( mxIsChar(arg_in_0), "ClothoidCurveMexWrapper: First argument must be a string" ) ;
      string cmd = mxArrayToString(arg_in_0) ;

      bool do_new = cmd == "new" ;
      ClothoidCurve * ptr = do_new ? DATA_NEW(arg_out_0) : DATA_GET(arg_in_1);

      if ( do_new || cmd == "build" ) {

        if ( do_new ) {
          MEX_ASSERT( nlhs == 1, "ClothoidCurveMexWrapper, expected 1 output, nlhs = " << nlhs );
        } else {
          MEX_ASSERT( nlhs == 0, "ClothoidCurveMexWrapper, expected no output, nlhs = " << nlhs );
        }

        int_type kk = do_new ? 0 : 1 ;

        real_type x0(0), y0(0), theta0(0), k0(0), dk(0), L(0);

        if ( do_new && nrhs == 1 ) {
          // nothing to do
        } else if ( nrhs == 1+kk ) {

          #define CMD "ClothoidCurveMexWrapper('build',OBJ,struct): "

          MEX_ASSERT( mxIsStruct(arg_in_1), CMD << "Argument n. " << 1+kk << " must be a struct" ) ;

          mxArray * mx_x0     = mxGetField( prhs[1+kk], 0, "x0" ) ;
          mxArray * mx_y0     = mxGetField( prhs[1+kk], 0, "y0" ) ;
          mxArray * mx_theta0 = mxGetField( prhs[1+kk], 0, "theta0" ) ;
          mxArray * mx_k0     = mxGetField( prhs[1+kk], 0, "k0" ) ;
          mxArray * mx_dk     = mxGetField( prhs[1+kk], 0, "dk" ) ;

          MEX_ASSERT( mx_x0     != nullptr, CMD "Field `x0` is missing" );
          MEX_ASSERT( mx_y0     != nullptr, CMD "Field `y0` is missing" );
          MEX_ASSERT( mx_theta0 != nullptr, CMD "Field `theta0` is missing" );
          MEX_ASSERT( mx_k0     != nullptr, CMD "Field `k0` is missing" );
          MEX_ASSERT( mx_dk     != nullptr, CMD "Field `dk` is missing" );

          x0     = getScalarValue( mx_x0,     CMD "Field `x0` must be a real double scalar" ) ;
          y0     = getScalarValue( mx_y0,     CMD "Field `y0` must be a real double scalar" ) ;
          theta0 = getScalarValue( mx_theta0, CMD "Field `theta0` must be a real double scalar" ) ;
          k0     = getScalarValue( mx_k0,     CMD "Field `k0` must be a real double scalar" ) ;
          dk     = getScalarValue( mx_dk,     CMD "Field `dk` must be a real double scalar" ) ;

          #undef CMD

        } else if ( nrhs == 7+kk ) {

          #define CMD "ClothoidCurveMexWrapper('build',OBJ,x0,y0,theta0,k0,dk,L): "

          x0     = getScalarValue(prhs[1+kk],CMD "Error in reading x0") ;
          y0     = getScalarValue(prhs[2+kk],CMD "Error in reading y0") ;
          theta0 = getScalarValue(prhs[3+kk],CMD "Error in reading theta0") ;
          k0     = getScalarValue(prhs[4+kk],CMD "Error in reading k0") ;
          dk     = getScalarValue(prhs[5+kk],CMD "Error in reading dk") ;
          L      = getScalarValue(prhs[6+kk],CMD "Error in reading L") ;
        } else {
          MEX_ASSERT( false, CMD  << "expected " << kk+1 << " or " << kk+7 <<
                             " inputs, nrhs = " << nrhs) ;
        }

        ptr->build( x0, y0, theta0, k0, dk, L );

        #undef CMD

      } else if ( cmd == "build_G1" || cmd == "build_G1_D") {

        #define CMD "ClothoidCurveMexWrapper('build_G1[_D]',OBJ,x0,y0,theta0,x1,y1,theta1): "

        MEX_ASSERT( nrhs == 8 , CMD "expected 8 inputs, nrhs = " << nrhs) ;

        real_type x0(0), y0(0), theta0(0), x1(0), y1(0), theta1(0);

        x0     = getScalarValue( arg_in_2, CMD "Error in reading x0" ) ;
        y0     = getScalarValue( arg_in_3, CMD "Error in reading y0" ) ;
        theta0 = getScalarValue( arg_in_4, CMD "Error in reading theta0" ) ;
        x1     = getScalarValue( arg_in_5, CMD "Error in reading x1" ) ;
        y1     = getScalarValue( arg_in_6, CMD "Error in reading y1" ) ;
        theta1 = getScalarValue( arg_in_7, CMD "Error in reading theta1" ) ;

        int iter ;
        if ( cmd == "build_G1" ) {
          MEX_ASSERT( nlhs == 0 || nlhs == 1, CMD "expected 1 or no outputs, nlhs = " << nlhs ) ;
          iter = ptr->build_G1( x0, y0, theta0, x1, y1, theta1 );
          if ( nlhs == 1 ) setScalarInt( arg_out_0, iter ) ;
        } else {
          MEX_ASSERT( nlhs == 3 || nlhs == 4, CMD "expected 3 or 4 outputs, nlhs = " << nlhs ) ;
          double * L_D  = createMatrixValue( arg_out_0, 2, 1 );
          double * k_D  = createMatrixValue( arg_out_1, 2, 1 );
          double * dk_D = createMatrixValue( arg_out_2, 2, 1 );
          iter = ptr->build_G1_D( x0, y0, theta0, x1, y1, theta1, L_D, k_D, dk_D );
          if ( nlhs == 4 ) setScalarInt( arg_out_3, iter ) ;
        }

        #undef CMD

      } else if ( cmd == "build_forward" ) {

        #define CMD "ClothoidCurveMexWrapper('build_forward',OBJ,x0,y0,theta0,kappa0,x1,y1): "

        MEX_ASSERT( nrhs == 8 , CMD "expected 8 inputs, nrhs = " << nrhs) ;

        real_type x0(0), y0(0), theta0(0), kappa0(0), x1(0), y1(0);

        x0     = getScalarValue( arg_in_2, CMD "Error in reading x0" ) ;
        y0     = getScalarValue( arg_in_3, CMD "Error in reading y0" ) ;
        theta0 = getScalarValue( arg_in_4, CMD "Error in reading theta0" ) ;
        kappa0 = getScalarValue( arg_in_5, CMD "Error in reading kappa0" ) ;
        x1     = getScalarValue( arg_in_6, CMD "Error in reading x1" ) ;
        y1     = getScalarValue( arg_in_7, CMD "Error in reading y1" ) ;

        bool ok = ptr->build_forward(x0, y0, theta0, kappa0, x1, y1);

        // returns the status of the interpolation
        setScalarBool( arg_out_0, ok ) ;

        #undef CMD

      } else if ( cmd == "evaluate" ) {

        #define CMD "ClothoidCurveMexWrapper('evaluate',OBJ,s): "

        MEX_ASSERT( nrhs == 3 , CMD "expected 3 inputs, nrhs = " << nrhs) ;

        mwSize size;
        double const * sVals = getVectorPointer( arg_in_2, size, CMD "Error in reading s" );

        if ( nlhs == 4 ) {
          double * xVals     = createMatrixValue( arg_out_0, size, 1 );
          double * yVals     = createMatrixValue( arg_out_1, size, 1 );
          double * thetaVals = createMatrixValue( arg_out_2, size, 1 );
          double * kappaVals = createMatrixValue( arg_out_3, size, 1 );
          for ( mwSize i=0; i < size ; ++i )
            ptr->eval( sVals[i], thetaVals[i], kappaVals[i], xVals[i], yVals[i] );
        } else if ( nlhs == 2 ) {
          double * xVals = createMatrixValue( arg_out_0, size, 1 );
          double * yVals = createMatrixValue( arg_out_1, size, 1 );
          for ( mwSize i=0; i < size ; ++i )
            ptr->eval( sVals[i], xVals[i], yVals[i] );
        } else {
          MEX_ASSERT( false, CMD "expected 2 or 4 outputs, nlhs = " << nlhs ) ;
        }

        #undef CMD

      } else if ( cmd == "eval"    || cmd == "eval_D" ||
                  cmd == "eval_DD" || cmd == "eval_DDD" ) {

        if ( nrhs == 4 ) {

          #define CMD "ClothoidCurveMexWrapper('eval*',OBJ,s,t): "

          mwSize size, sizet ;
          real_type const * s = getVectorPointer( arg_in_2, size,  CMD "`s` expected to be a real vector" ) ;
          real_type const * t = getVectorPointer( arg_in_3, sizet, CMD "`t` expected to be a real vector" ) ;

          MEX_ASSERT( size == sizet || size == 1 || sizet ==1,
                      CMD " size(s) = " << size <<
                      " must be equal to size(t) = " << sizet <<
                      " or size(s|t) == 1" );

          mwSize incs = size  == 1 ? 0 : 1 ;
          mwSize inct = sizet == 1 ? 0 : 1 ;
          mwSize npts = max(size,sizet) ;

          #define LOOPXY1 for ( mwSize i = 0 ; i < npts ; ++i, s += incs, t += inct, pXY += 2 )
          #define LOOPXY2 for ( mwSize i = 0 ; i < npts ; ++i, s += incs, t += inct, ++pX, ++pY )

          if ( nlhs == 1 ) {
            real_type *pXY = createMatrixValue( arg_out_0, 2,size );
            if ( cmd == "eval" ) {
              LOOPXY1 ptr->eval( *s, *t, pXY[0], pXY[1] ) ;
            } else if ( cmd == "eval_D" ) {
              LOOPXY1 ptr->eval_D( *s, *t, pXY[0], pXY[1] ) ;
            } else if ( cmd == "eval_DD" ) {
              LOOPXY1 ptr->eval_DD( *s, *t, pXY[0], pXY[1] ) ;
            } else if ( cmd == "eval_DDD" ) {
              LOOPXY1 ptr->eval_DDD( *s, *t, pXY[0], pXY[1] ) ;
            } else {
              MEX_ASSERT(false, CMD "Unknown command: " << cmd );
            }
          } else if ( nlhs == 2 ) {
            real_type *pX = createMatrixValue( arg_out_0, 1,size );
            real_type *pY = createMatrixValue( arg_out_1, 1,size );
            if ( cmd == "eval" ) {
              LOOPXY2 ptr->eval( *s, *t, *pX, *pY ) ;
            } else if ( cmd == "eval_D" ) {
              LOOPXY2 ptr->eval_D( *s, *t, *pX, *pY ) ;
            } else if ( cmd == "eval_DD" ) {
              LOOPXY2 ptr->eval_DD( *s, *t, *pX, *pY ) ;
            } else if ( cmd == "eval_DDD" ) {
              LOOPXY2 ptr->eval_DDD( *s, *t, *pX, *pY ) ;
            } else {
              MEX_ASSERT(false, CMD "Unknown command: " << cmd );
            }
          } else {
            MEX_ASSERT( nlhs == 0, CMD "expected 1 or 2 outputs, nlhs = " << nlhs ) ;
          }

          #undef CMD
          #undef LOOPXY1
          #undef LOOPXY2

        } else if ( nrhs == 3 ) {

          #define CMD "ClothoidCurveMexWrapper('eval*',OBJ,s): "

          mwSize npts ;
          real_type const * s = getVectorPointer( arg_in_2, npts, CMD "`s` expected to be a real vector" ) ;

          #define LOOPXY1 for ( mwSize i = 0 ; i < npts ; ++i, ++s, pXY += 2 )
          #define LOOPXY2 for ( mwSize i = 0 ; i < npts ; ++i, ++s, ++pX, ++pY )

          if ( nlhs == 1 ) {
            real_type *pXY = createMatrixValue( arg_out_0, 2, npts );
            if ( cmd == "eval" ) {
              LOOPXY1 ptr->eval( *s, pXY[0], pXY[1] ) ;
            } else if ( cmd == "eval_D" ) {
              LOOPXY1 ptr->eval_D( *s, pXY[0], pXY[1] ) ;
            } else if ( cmd == "eval_DD" ) {
              LOOPXY1 ptr->eval_DD( *s, pXY[0], pXY[1] ) ;
            } else if ( cmd == "eval_DDD" ) {
              LOOPXY1 ptr->eval_DDD( *s, pXY[0], pXY[1] ) ;
            } else {
              MEX_ASSERT(false, CMD "Unknown command: " << cmd );
            }
          } else if ( nlhs == 2 ) {
            real_type *pX = createMatrixValue( arg_out_0, 1, npts );
            real_type *pY = createMatrixValue( arg_out_1, 1, npts );
            if ( cmd == "eval" ) {
              LOOPXY2 ptr->eval( *s, *pX, *pY ) ;
            } else if ( cmd == "eval_D" ) {
              LOOPXY2 ptr->eval_D( *s, *pX, *pY ) ;
            } else if ( cmd == "eval_DD" ) {
              LOOPXY2 ptr->eval_DD( *s, *pX, *pY ) ;
            } else if ( cmd == "eval_DDD" ) {
              LOOPXY2 ptr->eval_DDD( *s, *pX, *pY ) ;
            } else {
              MEX_ASSERT(false, CMD "Unknown command: " << cmd );
            }
          } else {
            MEX_ASSERT( nlhs == 0, CMD "expected 1 or 2 outputs, nlhs = " << nlhs ) ;
          }
          #undef CMD
          #undef LOOPXY1
          #undef LOOPXY2

        } else {
          MEX_ASSERT(false, "ClothoidCurveMexWrapper('eval*',OBJ,...) bad number of arguments, nrhs = " << nrhs );
        }

      } else if ( cmd == "getPars" ) {

        MEX_ASSERT(nrhs == 2, "ClothoidCurveMexWrapper('getPars',OBJ): expected 2 inputs, nrhs = " << nrhs);
        if ( nlhs > 0 ) setScalarValue(arg_out_0,ptr->xBegin()) ;
        if ( nlhs > 1 ) setScalarValue(arg_out_1,ptr->yBegin()) ;
        if ( nlhs > 2 ) setScalarValue(arg_out_2,ptr->thetaBegin()) ;
        if ( nlhs > 3 ) setScalarValue(arg_out_3,ptr->kappaBegin()) ;
        if ( nlhs > 4 ) setScalarValue(arg_out_4,ptr->dkappa()) ;
        if ( nlhs > 5 ) setScalarValue(arg_out_5,ptr->length()) ;

      } else if ( cmd == "xyBegin" ) {

        #define CMD "ClothoidCurveMexWrapper('xyBegin',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 2, CMD "expected 2 outputs, nlhs = " << nlhs );
        setScalarValue(arg_out_0, ptr->xBegin());
        setScalarValue(arg_out_1, ptr->yBegin());

        #undef CMD

      } else if ( cmd == "xyEnd" ) {

        #define CMD "ClothoidCurveMexWrapper('xyEnd',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 2, CMD "expected 2 outputs, nlhs = " << nlhs );
        setScalarValue(arg_out_0, ptr->xEnd());
        setScalarValue(arg_out_1, ptr->yEnd());

        #undef CMD

      } else if ( cmd == "xBegin" ) {

        #define CMD "ClothoidCurveMexWrapper('xBegin',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 outputs, nlhs = " << nlhs );
        setScalarValue(arg_out_0, ptr->xBegin());

        #undef CMD

      } else if ( cmd == "xEnd" ) {

        #define CMD "ClothoidCurveMexWrapper('xEnd',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 outputs, nlhs = " << nlhs);
        setScalarValue(arg_out_0, ptr->xEnd());

        #undef CMD

      } else if ( cmd == "yBegin" ) {

        #define CMD "ClothoidCurveMexWrapper('yBegin',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 outputs, nlhs = " << nlhs);
        setScalarValue(arg_out_0, ptr->yBegin());

        #undef CMD

      } else if ( cmd == "yEnd" ) {

        #define CMD "ClothoidCurveMexWrapper('yEnd',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 outputs, nlhs = " << nlhs);
        setScalarValue(arg_out_0, ptr->yEnd());

        #undef CMD

      } else if ( cmd == "thetaBegin" ) {

        #define CMD "ClothoidCurveMexWrapper('thetaBegin',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 outputs, nlhs = " << nlhs);
        setScalarValue(arg_out_0, ptr->thetaBegin());

        #undef CMD

      } else if ( cmd == "thetaEnd" ) {

        #define CMD "ClothoidCurveMexWrapper('thetaEnd',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 outputs, nlhs = " << nlhs);
        setScalarValue(arg_out_0, ptr->thetaEnd());

        #undef CMD

      } else if ( cmd == "kappaBegin" ) {

        #define CMD "ClothoidCurveMexWrapper('kappaBegin',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 outputs, nlhs = " << nlhs);
        setScalarValue(arg_out_0, ptr->kappaBegin());

        #undef CMD

      } else if ( cmd == "kappaEnd" ) {

        #define CMD "ClothoidCurveMexWrapper('kappaEnd',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 outputs, nlhs = " << nlhs);
        setScalarValue(arg_out_0, ptr->kappaEnd());

        #undef CMD

      } else if ( cmd == "kappa_D" ) {

        #define CMD "ClothoidCurveMexWrapper('kappa_D',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 outputs, nlhs = " << nlhs);
        setScalarValue(arg_out_0, ptr->dkappa());

        #undef CMD

      } else if ( cmd == "length" ) {

        #define CMD "ClothoidCurveMexWrapper('length',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 outputs, nlhs = " << nlhs);
        setScalarValue(arg_out_0, ptr->length());

        #undef CMD

      } else if ( cmd == "trim" ) {

        #define CMD "ClothoidCurveMexWrapper('trim',OBJ): "

        MEX_ASSERT(nrhs == 4, CMD "expected 4 inputs, nrhs = " << nrhs);

        real_type smin = getScalarValue(arg_in_2,CMD "Error in reading smin") ;
        real_type smax = getScalarValue(arg_in_3,CMD "Error in reading smax") ;

        ptr->trim(smin, smax);

        #undef CMD

      } else if ( cmd == "changeCurvilinearOrigin" ) {

        #define CMD "ClothoidCurveMexWrapper('changeCurvilinearOrigin',OBJ,s0,L): "

        MEX_ASSERT(nrhs == 4, CMD "expected 4 inputs, nrhs = " << nrhs);

        real_type s0 = getScalarValue(arg_in_2,CMD "Error in reading s0") ;
        real_type L  = getScalarValue(arg_in_3,CMD "Error in reading L") ;
        ptr->changeCurvilinearOrigin(s0,L);

        #undef CMD

      } else if ( cmd == "rotate" ) {

        #define CMD "ClothoidCurveMexWrapper('rotate',OBJ,angle,cx,cy): "

        MEX_ASSERT(nrhs == 5, CMD "expected 5 inputs, nrhs = " << nrhs);

        real_type angle = getScalarValue( arg_in_2, CMD "Error in reading angle" ) ;
        real_type cx    = getScalarValue( arg_in_3, CMD "Error in reading cx" ) ;
        real_type cy    = getScalarValue( arg_in_4, CMD "Error in reading cy" ) ;
        ptr->rotate(angle, cx, cy);

        #undef CMD

      } else if ( cmd == "translate" ) {

        #define CMD "ClothoidCurveMexWrapper('translate',OBJ,tx,ty): "

        MEX_ASSERT(nrhs == 4, CMD "expected 4 inputs, nrhs = " << nrhs);
        real_type tx = getScalarValue( arg_in_2, CMD "Error in reading tx" ) ;
        real_type ty = getScalarValue( arg_in_3, CMD "Error in reading ty" ) ;
        ptr->translate(tx, ty);

        #undef CMD

      } else if ( cmd == "changeOrigin" ) {

        #define CMD "ClothoidCurveMexWrapper('changeOrigin',OBJ,newX0,newY0): "

        MEX_ASSERT(nrhs == 4, CMD "expected 4 inputs, nrhs = " << nrhs);
        real_type newX0 = getScalarValue( arg_in_2, CMD "Error in reading newX0" ) ;
        real_type newY0 = getScalarValue( arg_in_3, CMD "Error in reading newY0" ) ;
        ptr->changeOrigin(newX0, newY0);

        #undef CMD

      } else if ( cmd == "scale" ) {

        #define CMD "ClothoidCurveMexWrapper('scale',OBJ,s): "

        MEX_ASSERT(nrhs == 3, CMD "expected 3 inputs, nrhs = " << nrhs);
        real_type s = getScalarValue( arg_in_2, CMD "Error in reading s" ) ;
        ptr->scale(s);

        #undef CMD

      } else if ( cmd == "reverse" ) {

        #define CMD "ClothoidCurveMexWrapper('reverse',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        ptr->reverse();

        #undef CMD

      } else if ( cmd == "infinity" ) {

        #define CMD "ClothoidCurveMexWrapper('infinity',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 4, CMD "expected 4 output, nlhs = " << nlhs);

        real_type xp, yp, xm, ym ;
        ptr->Pinfinity( xp, yp, true );
        ptr->Pinfinity( xm, ym, false );

        setScalarValue(arg_out_0, xp ) ;
        setScalarValue(arg_out_1, yp );
        setScalarValue(arg_out_2, xm );
        setScalarValue(arg_out_3, ym );

        #undef CMD

      } else if ( cmd == "distance" ) {

        #define CMD "ClothoidCurveMexWrapper('distance',OBJ,x,y): "
        MEX_ASSERT( nrhs == 4, CMD "expected 4 input, nrhs = " << nrhs);
        if ( nlhs > 0 ) {
          MEX_ASSERT(nlhs <= 2, CMD "expected 1 or 2 output, nlhs = " << nlhs );
          mwSize nrx, ncx, nry, ncy;
          real_type const * x = getMatrixPointer( arg_in_2, nrx, ncx, CMD "`x` expected to be a real vector/matrix" ) ;
          real_type const * y = getMatrixPointer( arg_in_3, nry, ncy, CMD "`y` expected to be a real vector/matrix" ) ;
          MEX_ASSERT( nrx == nry && ncx == ncy,
                      CMD "`x` and `y` expected to be of the same size, found size(x) = " <<
                      nrx << " x " << nry << " size(y) = " << nry << " x " << ncy );

          real_type * dst = createMatrixValue( arg_out_0, nrx, ncx ) ;

          mwSize size = nrx*ncx ;
          if ( nlhs > 1 ) {
            real_type * s = createMatrixValue( arg_out_1, nrx, ncx ) ;
            for ( mwSize i = 0 ; i < size ; ++i )
              *dst++ = ptr->distance( *x++, *y++, *s++ ) ;
          } else {
            for ( mwSize i = 0 ; i < size ; ++i )
              *dst++ = ptr->distance( *x++, *y++ ) ;
          }
        }
        #undef CMD

      } else if ( cmd == "distanceBySample" ) {

        #define CMD "ClothoidCurveMexWrapper('distance',OBJ,x,y,ds): "
        MEX_ASSERT( nrhs == 5, CMD "expected 5 input, nrhs = " << nrhs);
        if ( nlhs > 0 ) {
          MEX_ASSERT(nlhs <= 2, CMD "expected 1 or 2 output, nlhs = " << nlhs );
          mwSize nrx, ncx, nry, ncy;
          real_type const * x = getMatrixPointer( arg_in_2, nrx, ncx, CMD "`x` expected to be a real vector/matrix" ) ;
          real_type const * y = getMatrixPointer( arg_in_3, nry, ncy, CMD "`y` expected to be a real vector/matrix" ) ;
          MEX_ASSERT( nrx == nry && ncx == ncy,
                      CMD "`x` and `y` expected to be of the same size, found size(x) = " <<
                      nrx << " x " << nry << " size(y) = " << nry << " x " << ncy );
          real_type ds = getScalarValue( arg_in_4, CMD "`ds` expected to be a real scalar" );
          MEX_ASSERT( ds > 0, CMD "`ds` = " << ds << " must be a positive number" );

          real_type * dst = createMatrixValue( arg_out_0, nrx, ncx ) ;

          mwSize size = nrx*ncx ;
          if ( nlhs > 1 ) {
            real_type * s = createMatrixValue( arg_out_1, nrx, ncx ) ;
            for ( mwSize i = 0 ; i < size ; ++i )
              *dst++ = ptr->distanceBySample( ds, *x++, *y++, *s++ ) ;
          } else {
            for ( mwSize i = 0 ; i < size ; ++i )
              *dst++ = ptr->distanceBySample( ds, *x++, *y++ ) ;
          }
        }
        #undef CMD

      } else if ( cmd == "closestPoint" ) {

        #define CMD "ClothoidCurveMexWrapper('closestPoint',OBJ,x,y): "
        MEX_ASSERT( nrhs == 4, CMD "expected 4 input, nrhs = " << nrhs);
        MEX_ASSERT( nlhs == 4, CMD "expected 4 outputs, nlhs = " << nlhs ) ;

        mwSize nrx, ncx, nry, ncy;
        real_type const * x = getMatrixPointer( arg_in_2, nrx, ncx,
                              CMD "`x` expected to be a real vector/matrix" ) ;
        real_type const * y = getMatrixPointer( arg_in_3, nry, ncy,
                              CMD "`y` expected to be a real vector/matrix" ) ;
        MEX_ASSERT( nrx == nry && ncx == ncy,
                    CMD "`x` and `y` expected to be of the same size, found size(x) = " <<
                    nrx << " x " << nry << " size(y) = " << nry << " x " << ncy );

        real_type * X   = createMatrixValue( arg_out_0, nrx, ncx ) ;
        real_type * Y   = createMatrixValue( arg_out_1, nrx, ncx ) ;
        real_type * S   = createMatrixValue( arg_out_2, nrx, ncx ) ;
        real_type * dst = createMatrixValue( arg_out_3, nrx, ncx ) ;

        mwSize size = nrx*ncx ;
        for ( mwSize i = 0 ; i < size ; ++i )
          *dst++ = ptr->closestPoint( *x++, *y++, *X++, *Y++, *S++ ) ;

        #undef CMD

      } else if ( cmd == "closestPointBySample" ) {

        #define CMD "ClothoidCurveMexWrapper('closestPointBySample',OBJ,x,y,ds): "
        MEX_ASSERT( nrhs == 5, CMD "expected 5 input, nrhs = " << nrhs);
        MEX_ASSERT( nlhs == 4, CMD "expected 4 outputs, nlhs = " << nlhs ) ;

        mwSize nrx, ncx, nry, ncy;
        real_type const * x = getMatrixPointer( arg_in_2, nrx, ncx,
                              CMD "`x` expected to be a real vector/matrix" ) ;
        real_type const * y = getMatrixPointer( arg_in_3, nry, ncy,
                              CMD "`y` expected to be a real vector/matrix" ) ;
        MEX_ASSERT( nrx == nry && ncx == ncy,
                    CMD "`x` and `y` expected to be of the same size, found size(x) = " <<
                    nrx << " x " << nry << " size(y) = " << nry << " x " << ncy );

        real_type ds = getScalarValue( arg_in_4, CMD "`ds` expected to be a real scalar" );
        MEX_ASSERT( ds > 0, CMD "`ds` = " << ds << " must be a positive number" );

        real_type * X   = createMatrixValue( arg_out_0, nrx, ncx ) ;
        real_type * Y   = createMatrixValue( arg_out_1, nrx, ncx ) ;
        real_type * S   = createMatrixValue( arg_out_2, nrx, ncx ) ;
        real_type * dst = createMatrixValue( arg_out_3, nrx, ncx ) ;

        mwSize size = nrx*ncx ;
        for ( mwSize i = 0 ; i < size ; ++i )
          *dst++ = ptr->closestPointBySample( ds, *x++, *y++, *X++, *Y++, *S++ ) ;

        #undef CMD

      } else if ( cmd == "intersect_line"   ||
                  cmd == "intersect_circle" ||
                  cmd == "intersect_clothoid" ||
                  cmd == "intersect_clothoid_list" ) {

        #define CMD "ClothoidCurveMexWrapper('intersect_*',OBJ,OBJ2): "
        MEX_ASSERT( nrhs == 3, CMD "expected 3 input, nrhs = " << nrhs );
        MEX_ASSERT( nlhs == 2, CMD "expected 2 outputs, nlhs = " << nlhs ) ;

        std::vector<real_type> s1, s2 ;
        int_type               max_iter  = 10 ;
        real_type              tolerance = 1e-8 ;

        if ( cmd == "intersect_line" ) {
          LineSegment const * ptr1 = convertMat2Ptr<LineSegment>(arg_in_2);
          ptr->intersect( *ptr1, s1, s2, max_iter, tolerance );
        } else if (  cmd == "intersect_circle" ) {
          CircleArc const * ptr1 = convertMat2Ptr<CircleArc>(arg_in_2);
          ptr->intersect( *ptr1, s1, s2, max_iter, tolerance );
        } else if (  cmd == "intersect_clothoid" ) {
          ClothoidCurve const * ptr1 = convertMat2Ptr<ClothoidCurve>(arg_in_2);
          ptr->intersect( *ptr1, s1, s2, max_iter, tolerance );
        } else if ( cmd == "intersect_clothoid_list" ) {
          // exchange intersection
          ClothoidList const * ptr1 = convertMat2Ptr<ClothoidList>(arg_in_2);
          ptr1->intersect( *ptr, s2, s1, max_iter, tolerance );
        }

        real_type * S1 = createMatrixValue( arg_out_0, s1.size(), 1 ) ;
        real_type * S2 = createMatrixValue( arg_out_1, s2.size(), 1 ) ;

        std::copy( s1.begin(), s1.end(), S1 ) ;
        std::copy( s2.begin(), s2.end(), S2 ) ;

        #undef CMD

      } else if ( cmd == "bbox" ) {

        #define CMD "ClothoidCurveMexWrapper('bbox', OBJ, max_angle, max_size [,offs]): "

        MEX_ASSERT(nrhs == 4 || nrhs == 5, CMD "expected 4 or 5 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 1, CMD "expected 1 output, nlhs = " << nlhs);

        real_type max_angle = getScalarValue( arg_in_2, CMD "Error in reading max_angle" ) ;
        real_type max_size  = getScalarValue( arg_in_3, CMD "Error in reading max_size" ) ;
        real_type offs      = 0 ;
        if ( nrhs == 5 ) offs = getScalarValue( arg_in_4, CMD "Error in reading offs" ) ;

        vector<ClothoidCurve::bbData> bb ;
        ptr->bbSplit( max_angle, max_size, offs, bb ) ;

        plhs[0] = mxCreateDoubleMatrix(6, bb.size(), mxREAL);
        double * pT = mxGetPr(plhs[0]);
        for ( int i = 0 ; i < bb.size() ; ++i ) {
          Triangle2D const & t = bb[i].t ;
          *pT++ = t.x1() ; *pT++ = t.y1() ;
          *pT++ = t.x2() ; *pT++ = t.y2() ;
          *pT++ = t.x3() ; *pT++ = t.y3() ;
        }
        #undef CMD

      } else if ( cmd == "findST" ) {

        #define CMD "ClothoidCurveMexWrapper('findST',OBJ,x,y): "
        MEX_ASSERT( nrhs == 4, CMD "expected 4 input, nrhs = " << nrhs );
        MEX_ASSERT( nlhs == 2, CMD "expected 2 output, nlhs = " << nlhs );
        mwSize nrx, ncx, nry, ncy;
        real_type const * x = getMatrixPointer( arg_in_2, nrx, ncx,
                              CMD "`x` expected to be a real vector/matrix" ) ;
        real_type const * y = getMatrixPointer( arg_in_3, nry, ncy,
                              CMD "`y` expected to be a real vector/matrix" ) ;
        MEX_ASSERT( nrx == nry && ncx == ncy,
                    CMD "`x` and `y` expected to be of the same size, found size(x) = " <<
                    nrx << " x " << nry << " size(y) = " << nry << " x " << ncy );

        real_type * s = createMatrixValue( arg_out_0, nrx, ncx ) ;
        real_type * t = createMatrixValue( arg_out_1, nrx, ncx ) ;

        mwSize size = nrx*ncx ;
        for ( mwSize i = 0 ; i < size ; ++i )
          ptr->findST( *x++, *y++, *s++, *t++ ) ;

        #undef CMD

      } else if ( cmd == "info" ) {

        #define CMD "ClothoidCurveMexWrapper('info',OBJ): "

        MEX_ASSERT( nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs ) ;
        MEX_ASSERT( nlhs == 0, CMD "expected NO outputs, nlhs = " << nlhs ) ;
        
        ptr->info(cout) ;

        #undef CMD

      } else if ( cmd == "delete" ) {

        #define CMD "ClothoidCurveMexWrapper('delete',OBJ): "

        MEX_ASSERT(nrhs == 2, CMD "expected 2 inputs, nrhs = " << nrhs);
        MEX_ASSERT(nlhs == 0, CMD "expected no output, nlhs = " << nlhs);

        // Destroy the C++ object
        DATA_DELETE(arg_in_1);

        #undef CMD

      } else if ( cmd == "copy" ) {

        #define CMD "ClothoidCurveMexWrapper('copy',OBJ,OBJ1): "
        MEX_ASSERT(nrhs == 3, CMD "expected 3 inputs, nrhs = " << nrhs );
        MEX_ASSERT(nlhs == 0, CMD "expected no output, nlhs = " << nlhs );

        ClothoidCurve const * C = convertMat2Ptr<ClothoidCurve>(arg_in_2);
        ptr->copy(*C) ;

        #undef CMD

        // Warn if other commands were ignored
      } else {
        MEX_ASSERT(false, "Unknown command: " << cmd );
      }

    } catch ( std::exception const & e ) {
    	mexErrMsgTxt(e.what()) ;

    } catch (...) {
  	  mexErrMsgTxt("clothoid failed\n") ;
    }
  }
}
