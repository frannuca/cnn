#ifndef SERIES_H
#define SERIES_H

#include <map>
#include <vector>
#include <algorithm>
#include <exception>
#include <utility>
#include <functional>
#include <optional>
#include <memory>
#include <iterator>
#include <assert.h>
#include <set>
#include "axis.hpp"
#include <exception>

namespace AIX{namespace Data{

    
    /** Key Value containiner type.
     * 
     * Series of key and values sorted ascendent/descendent with extenden mathematical operations: +, -, *, /, scalar sum, scala mult, map, filter etc...
     */
    template<typename K, typename V>
    class Series{
        public:
        
        using TAxis = Axis<K,V>;
        using Container = std::vector<TAxis>;   
        using const_iterator =  typename Container::const_iterator;
        using iterator = typename  Container::iterator;

        Series(const std::vector<K>& keys,const std::vector<V>& values);                                
        Series(std::vector<Axis<K,V>> && s):_data(std::forward<TAxis>(s)){};
        Series(const std::vector<Axis<K,V>> & s):_data(s){};
        
        Series(const Series<K,V>& that);
        Series(Series<K,V>&& that);

        Series<K,V>& operator=(const Series<K,V>& that);
        Series<K,V>& operator=(const Axis<K,V>& axis);
        Series<K,V>& operator=(Series<K,V>&& that);

        void sort(bool descendentorder=true);
        V& operator[](const K& i);
        V operator[](const K& i) const;
        std::vector<K> Keys() const;
        
        iterator begin()  {return  _data.begin();}
        iterator end()    {return  _data.end();}
        
        const_iterator begin() const  {return  _data.begin();}
        const_iterator end()   const  {return  _data.end();}
        
        
        std::size_t size(){return _data.size();}
        
        template<class A, class B>
        friend  Series<A,B> operator+(const Series<A,B>& a, const Series<A,B>& b);

        template<class A, class B> 
        friend class Series_Iterator;

        protected:        
        Container _data; 
        std::optional<bool> sortedDescendent;   

    };

   
    template<class K, class V>
    Series<K,V>::Series(const std::vector<K>& keys, const std::vector<V>& values){
        assert(keys.size() == values.size());            
        std::transform(keys.begin(),keys.end(),values.begin(),std::back_inserter(_data),
                            [](const K& a,const V& b){return TAxis{a,b};});           
    }
    

    template<class K, class V>
    Series<K,V>::Series(const Series<K,V>& that){
        _data = that._data;
    }
    
    template<class K, class V>
    Series<K,V>::Series(Series<K,V>&& that){
            _data = std::move(that._data);
    }

    template<class K, class V>
    Series<K,V>& Series<K,V>::operator=(const Series<K,V>& that){
        _data = that._data;
        return *this;
    }

    template<class K, class V>
    Series<K,V>& Series<K,V>::operator=(const Axis<K,V>& that){
        _data = that;
        return *this;
    }


    template<class K, class V>
    Series<K,V>& Series<K,V>::operator=(Series<K,V>&& that){
        _data = std::move(that._data);
        return *this;
    }

    template<class K, class V>
    V& Series<K,V>::operator[](const K& i){
       
       
       auto ilow = std::lower_bound(begin(),end(),i);
       if(ilow == end()){
           throw "Not found";
       }
       
       return ilow->y();
    }

    template<class K, class V>
    V Series<K,V>::operator[](const K& i) const{
        const_iterator p;
       if(!sortedDescendent){
           p = std::find(begin(),end(),i);
       }
       else{
            const_iterator p1,p2;
            std::tie(p1,p2)=std::equal_range(begin(),end(),i);    
            if(p1->x() == i){
                p = p1;
            }
            else if(p2->x() == i){
                p=p2;
            }
            else{
                p = end();
            }
       }
        
        if(p == end()){            
            throw std::invalid_argument("Key not present in series container");
        }
       return **p;
    }

    template<class K, class V>
    std::vector<K> Series<K,V>::Keys() const{
        std::vector<K> keys;
        std::transform(_data.begin(),_data.end(),std::back_inserter(keys),[](auto& a){return a.x;});
        return keys;
    }

    template<class K, class V>
    void Series<K,V>::sort(bool descendentorder){
        if(sortedDescendent && *sortedDescendent==descendentorder){
            return;
        }

        if(descendentorder){
            std::sort(begin(),end(),[](auto& a, auto& b){return a.x()<b.x();});
        }
        else{
            std::sort(begin(),end(),[](auto& a, auto& b){return a.x()>b.x();});
        }
        sortedDescendent= descendentorder;
    }

    template<class K, class V>
    Series<K,V> operator+(const Series<K,V>& a, const Series<K,V>& b){
        std::vector<Axis<K,V>> intersection,symdiff;
        std::set_intersection(a.begin(),a.end(),b.begin(),b.end(),std::back_inserter(intersection));

        std::vector<Axis<K,V>> sumr;    
        for(auto p:intersection){
            K k = (K)p;
            V v = a[k]+b[k];
            sumr.push_back(Axis<K,V>(k,v));
        }

        Series<K,V> r(sumr);
        return r;
        
    }
}
}
#endif