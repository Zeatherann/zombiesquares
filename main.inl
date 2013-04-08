template<typename Type>inline void operator+=(vector<Type>& L,const vector<Type>& R){
    L.insert(L.end(),R.begin(),R.end());
}
template<typename T> std::string ToString(T number){
    std::stringstream S;
    S<<number;
    return S.str();
}
template<typename KeyType,typename ValueType>inline const KeyType* InMap(const map<KeyType*,ValueType>& Map,const KeyType& Element){
    for(const pair<const KeyType*,ValueType>& Iter:Map){
        if(*Iter.first==Element)return Iter.first;
    }
    return NULL;
}
template<typename T> T ToNumber(std::string str){
    std::stringstream S;
    S<<str;
    T Ret;
    S>>Ret;
    return Ret;
}
template<typename Type>inline set<Type>& operator+=(set<Type>& L,const set<Type>& R){
    L.insert(R.begin(),R.end());
    return L;
}
template<typename Type1,typename Type2>inline pair<Type1,Type2> operator+(const pair<Type1,Type2>& L,const pair<Type1,Type2>& R){
    return pair<Type1,Type2>(L.first+R.first,L.second+R.second);
}
template<typename Type1,typename Type2>inline pair<Type1,Type2> operator-(const pair<Type1,Type2>& L,const pair<Type1,Type2>& R){
    return pair<Type1,Type2>(L.first-R.first,L.second-R.second);
}
template<typename Type1,typename Type2>inline ostream& operator<<(ostream& L,const pair<Type1,Type2>& R){
    return L<<"("<<R.first<<", "<<R.second<<")";
}
template<typename Type>inline sf::Vector2<Type> operator*(const sf::Vector2<Type>& L,const sf::Vector2<Type>& R){
    return sf::Vector2<Type>(L.x*R.x,L.y*R.y);
}
