/*
Test assignment
Thank you for participating in our recruiting test. This will be a C++ programming test!

How to prepare for this test
Task Description
interval_map<K,V> is a data structure that associates keys of type K with values of type V. It is designed to be used efficiently in situations where intervals of consecutive keys are associated with the same value. Your task is to implement the assign member function of this data structure, which is outlined below.

interval_map<K, V> is implemented on top of std::map. For more information on std::map, you may refer to cppreference.com.

Each key-value-pair (k,v) in interval_map<K,V>::m_map means that the value v is associated with all keys from k (including) to the next key (excluding) in m_map. The member interval_map<K,V>::m_valBegin holds the value that is associated with all keys less than the first key in m_map.

Example: Let M be an instance of interval_map<int,char> where

M.m_valBegin=='A',
M.m_map=={ (1,'B'), (3,'A') },
Then M represents the mapping

...
-2 -> 'A'
-1 -> 'A'
0 -> 'A'
1 -> 'B'
2 -> 'B'
3 -> 'A'
4 -> 'A'
5 -> 'A'
...
The representation in the std::map must be canonical, that is, consecutive map entries must not contain the same value: ..., (3,'A'), (5,'A'), ... is not allowed. Likewise, the first entry in m_map must not contain the same value as m_valBegin. Initially, the whole range of K is associated with a given initial value, passed to the constructor of the interval_map<K,V> data structure.

Key type K

besides being copyable and assignable, is less-than comparable via operator<, and
does not implement any other operations, in particular no equality comparison or arithmetic operators.
Value type V

besides being copyable and assignable, is equality-comparable via operator==, and
does not implement any other operations.
*/

#include <map>
template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
        //sanity check
        if (!(keyBegin < keyEnd)) {
            return;
        }

        //start with keyEnd?
        //yes, to cover case where begin and end are between one interval

        //check if endpoint
        if (m_map.count(keyEnd) == 1) {
            //in case next interval is same
            if (m_map[keyEnd] == val) {
                m_map.erase(keyEnd);
            }
            //else do nothing
        }
        
        //can lump these two together
        
        //check if same value as interval its in
        //how to call the DEFINED operator??
        else if (operator[](keyEnd) == val){
            //leave as is
        }
        
        //if value is different from interval 
        else {
            //use defined operator []
            //insert rest of interval
            m_map.insert(std::pair<K,V>(keyEnd, operator[](keyEnd)));
        }
        
        //now do keyBegin
        //if endpoint, need to check if value before is different
        //can set up itBegin in the process of this
        auto itBegin = m_map.lower_bound(keyBegin);
        if (m_map.count(keyBegin) == 1) {
            //then itBegin is the point
            V temp1 = val;
            V temp2 = val;
            
            //find different way to get prior value...
            //cant just do --itBegin;
            if (itBegin == m_map.begin()) {
                temp2 = m_valBegin;
            }
            else {
                --itBegin;
                temp2 = itBegin->second;
                ++itBegin;
            }
            
            
            if (temp1 == temp2) {
                //gonna be deleted later
            }
            else {
                ++itBegin;
                //not gonna be deleted
                //I hope this [] works
                m_map[keyBegin] = val;
            }
        }
        
        //check if value is the same
        else if (operator[](keyBegin) == val) {
            //leave as is
        }
        
        //if value is different 
        else {
            //insert the input value
            m_map.insert(std::pair<K,V>(keyBegin, val));
            //shouldnt affect iterator
            //but just to be safe
            itBegin = m_map.upper_bound(keyBegin);
        }
        
        
        
        //now erase all in between
        //cant just do auto itBegin = ((m_map.upper_bound(keyBegin));
        auto itEnd = m_map.lower_bound(keyEnd);
        m_map.erase(itBegin, itEnd);
        return;

	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.