//
//  Store.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#ifndef Store_hpp
#define Store_hpp

#include "Bank.h"
#include <string>
#include <vector>

typedef struct _result {
    bool success;
    int purchased;
    float paid;
} PurchaseResult;

class Store
{
public:
    typedef struct StoreItem {
        int id;
        float price;
        std::string shortdesc;
        std::string description;
        
        StoreItem(int i, float p, std::string s, std::string d) : id(i), price(p), shortdesc(s), description(d) {};
    } StoreItem;
    
    Store();
    ~Store();
    std::vector<StoreItem> getItems() { return _items; }
    PurchaseResult purchase(int itemId);

private:
    std::vector<StoreItem> _items;
};

#endif /* Store_hpp */
