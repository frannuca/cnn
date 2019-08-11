#include "layer.hpp"
#include <algorithm>

namespace cnn{

    Layer::Layer(size_t number_of_cells,Cell::TActivation f, Cell::TActivation df):Ncells(number_of_cells){
        auto cellproto = Cell(f,df);
        for(size_t i=0;i<number_of_cells;++i) _cells.push_back(cellproto);
    }
    size_t Layer::size(){
        return Ncells;
    }

    arma::vec  Layer::forward(const arma::vec& x, const arma::mat& W){
        auto s = arma::conv_to<std::vector<double>>::from( 
                            arma::conv_to<arma::colvec>::from(W.submat(0,0,arma::SizeMat(W.n_rows,x.size())) * x) + W.col(x.size()+1)
                            );
        int i=0;
        std::vector<double> gf,f;
        for(auto cell:_cells){
            cell.compute(s[i++]);
            double out,dout;
            std::tie(out,dout) = cell.getData();
            f.push_back(out);
            gf.push_back(dout);                        
        }

        outputs = arma::vec(f);
        doutputs = arma::vec(gf);
        f.push_back(1.0);
        outputs_1 = arma::vec(f);

        return outputs.get();
    }

    const arma::vec& Layer::getOutputs(bool extended){
        if(!extended){
            return outputs.get();
        }
        else{
            return outputs_1.get();
        }
        
    }

    arma::vec Layer::backward(){        
        return doutputs.get();
    }

}