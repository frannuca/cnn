#include <iostream>
#include <memory>
#include <functional>
#include <math.h>
#include <armadillo>
#include "mlp/commons.hpp"
#include "mlp/inetwork.hpp"
#include "mlp/ffnn.hpp"
#include "mlp/ffnn_newton.hpp"
#include "mlp/ffnnbuilder.hpp"
#include "activations.hpp"
#include "mlp/layer.hpp"
#include "xor.hpp"

using namespace AIX;
using namespace AIX::MLP;

int main(int argc, char** argv )
{
    size_t number_of_cells = 2;
    double c = 1.0;
    std::shared_ptr<IActivation> sigmoid(new HyperbolicTangent(c));
    std::shared_ptr<IActivation> exponential(new Exponential(1.0));    
    
    auto floss = [](const arma::vec& x, const arma::vec& y){
                std::vector<double> vy = arma::conv_to<std::vector<double>>::from(y);

                auto piter = std::find(vy.begin(),vy.end(),1.0);
                int n = std::distance(vy.begin(),piter);                
                return -std::log(x(n));
    };
    
    auto dfloss = [](const arma::vec& x, const arma::vec& y){
         std::vector<double> vy = arma::conv_to<std::vector<double>>::from(y);
         auto piter = std::find(vy.begin(),vy.end(),1.0);
         int n = std::distance(vy.begin(),piter);  
         arma::vec r(y.size());
         r(n) = -1.0/x(n);
         return r;
         };

    AIX::CrossEntropyLoss floss2;
    FFNN_RSPROP_Params rsparam(0.01,0.001,0.7,0.5,0.01);
    FFNN_Newton_Raphson_Params newtonparams(0.01);
    
    FFNNBuilder net(FFNN_TYPES::RSPROP,&rsparam);
    
    std::unique_ptr<INetwork> ffnn = net.withInputLayer(2)
                                        .withHiddenLayer(number_of_cells,sigmoid)                                        
                                        .withSoftmaxOutputLayer(4)
                                        .withLossFunctions(&floss2)
                                        .Build();
                
    //data set:
    
    TrainingSet tset = XORDS::GetXORSoftmaxSet();
    
    ffnn->train(tset,10000,0.01);

    std::cout<<"(0,0)->"<<(*ffnn)(tset[0].input)<<" expected->"<<tset[0].output<<std::endl;
    std::cout<<"(0,1)->"<<(*ffnn)(tset[1].input)<<" expected->"<<tset[1].output<<std::endl;
    std::cout<<"(1,0)->"<<(*ffnn)(tset[2].input)<<" expected->"<<tset[2].output<<std::endl;
    std::cout<<"(1,1)->"<<(*ffnn)(tset[3].input)<<" expected->"<<tset[3].output<<std::endl;
    

    std::cout<<"OK"<<std::endl;
}