#ifndef FFNN_H
#define FFNN_H
#include "layer.hpp"
#include <memory>
#include <functional>
#include <armadillo>
#include "inetwork.hpp"

namespace cnn{
 class FFNNBuilder;
 class FFNN: public INetwork{
        public:
            std::vector<std::unique_ptr<Layer>> _layers;
            std::unique_ptr<Layer> _outputLayer;
            std::function<double(const arma::vec&,const arma::vec&)> _floss;
            std::function<arma::vec(const arma::vec&,const arma::vec&)> _dfloss;
            mutable std::vector<arma::mat> _Ws;
            mutable std::vector<arma::mat> _dWs_accum;
            size_t _number_inputs;
            mutable boost::optional<double> _loss;
            mutable boost::optional<arma::vec> _dloss;
            mutable boost::optional<arma::vec> _input;
            mutable boost::optional<arma::vec> _input_1;
            mutable boost::optional<arma::vec> _output;            
            virtual void init(){};
            //friend class FFNNBuider;
        public:
            FFNN();
            void withInputLayer(size_t number_of_inputs);
            void withHiddenLayer(std::unique_ptr<Layer>);
            void withOutputLayer(std::unique_ptr<Layer>);
            void withLossFunctions(std::function<double(const arma::vec&,const arma::vec&)> loss,std::function<arma::vec(const arma::vec&, const arma::vec&)>grad_loss);
            
            double forward(const arma::vec& xin, const arma::vec& y) const override;
            virtual void backward() const override = 0;
            virtual void update(double totalerror) const  override = 0;
            arma::vec operator()(const arma::vec& x) const override;
            virtual double train(const TrainingSet& trainingset, size_t niter,double tol) const override; 
            virtual ~FFNN();
    };


}
#endif