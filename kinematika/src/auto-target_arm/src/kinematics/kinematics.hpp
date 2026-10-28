#pragma once
#include <vector>
#include <cmath>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

struct DH{
    double a, alpha, d, theta;
};

Matrix4d TM(double a, double alpha, double d, double theta){
    double ca = cos(alpha), sa = sin(alpha);
    double ct = cos(theta), st = sin(theta);
    Matrix4d T;
    T << ct, -st*ca, st*sa, a*ct,
         st, ct*ca, -ct*sa, a*st,
         0, sa, ca, d,
         0, 0, 0, 1;

    return T;
}

Matrix4d FK(const vector<DH>& dh, const VectorXd& q){
    Matrix4d A = Matrix4d::Identity();
    for(size_t i=0; i<dh.size(); ++i){
        A *= TM(dh[i].a, dh[i].alpha, dh[i].d, q[i]+dh[i].theta);
    }
    return A;
}

Vector3d position(const vector<DH>& dh, const VectorXd& q){
    return FK(dh, q).block<3,1>(0,3);
}

MatrixXd Jacobian(const vector<DH>& dh, const VectorXd& q){
    const double  geser = 0.000001;
    int n = q.size();
    MatrixXd J(3,n);
    Vector3d pos = position(dh, q);

    for(int i=0; i<n; i++){
        VectorXd q_new = q;
        q_new[i] += geser;
        J.col(i) = (position(dh, q_new) - pos)/geser;
    }

    return J;
}

bool IK(const vector<DH>& dh, const VectorXd& target, VectorXd& q){
    for(int i=0; i<10000; i++){
        Vector3d error = target - position(dh, q);

        if(error.norm() < 0.00001){
            return true;
        }

        MatrixXd J = Jacobian(dh, q);
        VectorXd dq = J.jacobiSvd(ComputeThinU | ComputeThinV).solve(error);
        
        double scale = 0.1;
        q += scale*dq;
    }

    return (target - position(dh, q)).norm() < 0.00001;
}