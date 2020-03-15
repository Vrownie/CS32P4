#include <vector>
#include <list>
#include <functional>
#include <iostream>

#ifndef ExpandableHashMap_h
#define ExpandableHashMap_h

// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    struct Item {
        Item(KeyType k, ValueType v) { key = k; value = v; }
        KeyType key;
        ValueType value;
    };
    std::vector<std::list<Item>> m_storage;
    double m_mlf;
    int m_nItems;
    int m_nBuckets;
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor) : m_storage(8)
{
    m_mlf = maximumLoadFactor;
    m_nItems = 0;
    m_nBuckets = 8;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap() {}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    m_storage.clear();
    m_storage.resize(8);
    m_nItems = 0;
    m_nBuckets = 8;
}

template<typename KeyType, typename ValueType>
inline int ExpandableHashMap<KeyType, ValueType>::size() const { return m_nItems; }

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    unsigned int hasher(const KeyType& k);
    unsigned int h = hasher(key);
    h %= m_nBuckets;
    
    bool repInd = false;
    for(auto ii = m_storage[h].begin(); ii != m_storage[h].end(); ii++) {
        if(ii->key == key) {
            ii->value = value;
            repInd = true;
            break;
        }
    }
    
    if(!repInd) {
        m_storage[h].push_back(Item(key,value));
        m_nItems++;
    }
    
    //Rehash
    if((double) m_nItems / (double) m_nBuckets > m_mlf) {
        std::list<Item> temp;
        
        for(int i = 0; i < m_storage.size(); i++) {
            temp.splice(temp.begin(), m_storage[i]);
            m_storage[i].clear();
        }
        
        m_nItems = 0;
        m_nBuckets *= 2;
        m_storage.clear();
        m_storage.resize(m_nBuckets);
        
        for (auto ii = temp.begin(); ii != temp.end(); ii++) {
            associate(ii->key, ii->value);
        }
    }
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int hasher(const KeyType& k);
    unsigned int h = hasher(key);
    h %= m_nBuckets;
    
    for(auto ii = m_storage[h].begin(); ii != m_storage[h].end(); ii++) {
        if(ii->key == key) return (&ii->value);
    }
    return nullptr;
}

#endif /* ExpandableHashMap_h */
