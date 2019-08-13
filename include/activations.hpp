#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

namespace cnn{
    class IActivation{
        public:
           virtual double f(const double&) = 0; 
           virtual double df(const double&) = 0; 
           virtual ~IActivation(){};
    };

    class Sigmoid:public IActivation{
        private:
            const double c;
        
        public:
           Sigmoid()=delete;
           Sigmoid(double c);
           double f(const double&) override; 
           double df(const double&) override; 
    };

       class ReLU:public IActivation{
        private:
            const double leak;
            const double slope;
        
        public:
           ReLU()=delete;
           ReLU(double leak_,double slope_);
           double f(const double&) override; 
           double df(const double&) override; 
    };
}
#endif