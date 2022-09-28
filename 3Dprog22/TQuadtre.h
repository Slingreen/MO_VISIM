#ifndef TQUADTRE_H
#define TQUADTRE_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <iterator>
//#include <utility>
#include "logger.h"

namespace gsml{

typedef std::pair<double,double> Point2D;

template <class ID, class T>
class QuadTre
{
private:
    Point2D m_a;
    Point2D m_b;
    Point2D m_c;
    Point2D m_d;

    QuadTre* m_sv;
    QuadTre* m_so;
    QuadTre* m_no;
    QuadTre* m_nv;

    std::list<std::pair<ID,T>> m_objects;

    bool isLeaf() const;

    std::vector<T> m_all_objects;

    void traverse_all(std::vector<T>& all_objects);

    class Logger *mLogger{nullptr};         //logger - Output Log in the application
public:
    QuadTre();
    QuadTre(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4);
    void init(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4);
    void subDivide(int n);
    void print() const;
    void print_all() const;
    QuadTre* insert(const Point2D& point, const ID& id, const T& object);
    QuadTre* find(const Point2D& p);
    const QuadTre* find(const Point2D& p) const;
    T& find(const ID& id);
    T& operator[] (ID id);

    typename std::vector<T>::iterator begin()
    {
        traverse_all(m_all_objects);
        return m_all_objects.begin();
    }
    typename std::vector<T>::iterator end()
    {
        return m_all_objects.end();
    }
};
}

template<class ID, class T>
gsml::QuadTre<ID, T>::QuadTre()
{
    m_a = Point2D(-1,-1);
    m_b = Point2D(1,-1);
    m_c = Point2D(1,1);
    m_d = Point2D(-1,1);

    m_sv = m_so = m_no = m_nv = nullptr;
    mLogger = Logger::getInstance();
}

template<class ID, class T>
gsml::QuadTre<ID, T>::QuadTre(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4)
    :m_a{v1}, m_b{v2}, m_c{v3}, m_d{v4},
      m_sv{nullptr}, m_so{nullptr}, m_no{nullptr}, m_nv{nullptr}
{
    //
    mLogger = Logger::getInstance();
}

template<class ID, class T>
void gsml::QuadTre<ID, T>::init(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4)
{
    m_a = v1; m_b = v2; m_c = v3; m_d = v4;
}

template<class ID, class T>
void gsml::QuadTre<ID, T>::subDivide(int n)
{
    if (n > 0) {

        Point2D v1 = Point2D((m_a.first + m_b.first)/2, (m_a.second + m_b.second)/2);
        Point2D v2 = Point2D((m_b.first + m_c.first)/2, (m_b.second + m_c.second)/2);
        Point2D v3 = Point2D((m_c.first + m_d.first)/2, (m_c.second + m_d.second)/2);
        Point2D v4 = Point2D((m_d.first + m_a.first)/2, (m_d.second + m_a.second)/2);
        Point2D m = Point2D((m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2);

        m_sv = new QuadTre (m_a, v1 , m, v4 ) ;
        m_sv->subDivide ( n-1);
        m_so = new QuadTre ( v1 , m_b, v2 , m) ;
        m_so->subDivide ( n-1);
        m_no = new QuadTre (m, v2 , m_c, v3 ) ;
        m_no->subDivide ( n-1);
        m_nv = new QuadTre ( v4 , m, v3 , m_d) ;
        m_nv->subDivide ( n-1);
    }

}

template<class ID, class T>
void gsml::QuadTre<ID, T>::print() const
{
    std::string tempString;
    Point2D m = Point2D((m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2);

    //std::cout << "senter = (" << m.first << ", " << m.second << ")" << std::endl;
    tempString = "senter = (" + std::to_string(m.first) + ", "  + std::to_string(m.second) + ")\n";
    for (auto it = m_objects.begin(); it != m_objects.end(); it++)
        tempString += it->first + ", " + it->second->getName() + "\n";
        //std::cout << it->first << ", " << it->second->getName() << std::endl;
    mLogger->logText(tempString);

}

template<class ID, class T>
void gsml::QuadTre<ID, T>::print_all() const
{
    print();
    if (m_sv) m_sv->print_all();
    if (m_sv) m_so->print_all();
    if (m_sv) m_no->print_all();
    if (m_sv) m_nv->print_all();
}

template<class ID, class T>
gsml::QuadTre<ID, T> *gsml::QuadTre<ID, T>::insert(const Point2D &point, const ID &id, const T &object)
{
    if (isLeaf())
    {
        m_objects.push_back(std::pair<ID,T>{id, object});
        return this;
    }
    else
    {
        Point2D m = Point2D{(m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2};

        if (point.second < m.second)
        {
            if (point.first < m.first)
                m_sv->insert(point, id, object);
            else
                m_so->insert(point, id, object);
        }
        else
        {
            if (point.first < m.first)
                m_nv->insert(point, id, object);
            else
                m_no->insert(point, id, object);
        }
    }
}

template<class ID, class T>
gsml::QuadTre<ID, T> *gsml::QuadTre<ID, T>::find(const Point2D &p)
{
    if (isLeaf())
        return this;
    else
    {
        Point2D m = Point2D{(m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2};

        if (p.second < m.second)
        {
            if (p.first < m.first)
                m_sv->find(p);
            else
                m_so->find(p);
        }
        else
        {
            if (p.first < m.first)
                m_nv->find(p);
            else
                m_no->find(p);
        }
    }
}

template<class ID, class T>
const gsml::QuadTre<ID, T> *gsml::QuadTre<ID, T>::find(const Point2D &p) const
{
    // samme som uten const??
    if (isLeaf())
        return this;
    else
    {
        Point2D m = Point2D{(m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2};

        if (p.second < m.second)
        {
            if (p.first < m.first)
                m_sv->find(p);
            else
                m_so->find(p);
        }
        else
        {
            if (p.first < m.first)
                m_nv->find(p);
            else
                m_no->find(p);
        }
    }
}

template<class ID, class T>
T &gsml::QuadTre<ID, T>::find(const ID &id)
{
    if (isLeaf() && m_objects.size()>0)
    {
        for (auto it = m_objects.begin(); it != m_objects.end(); it++)
        {
            if (it->first == id)
                return it->second;
        }
    }
    else if (m_sv)
            m_sv->find(id);
    else if (m_so)
            m_so->find(id);
    else if (m_no)
            m_no->find(id);
    else if (m_nv)
            m_nv->find(id);
}

template<class ID, class T>
T &gsml::QuadTre<ID, T>::operator[](ID id)
{
    if (isLeaf())
    {
        for (auto it = m_objects.begin(); it != m_objects.end(); it++)
        {
            if (it->first == id)
                return it->second;
        }
    }
}

template<class ID, class T>
bool gsml::QuadTre<ID, T>::isLeaf() const
{
    return m_sv == nullptr && m_so == nullptr && m_no == nullptr && m_nv == nullptr;
}

template<class ID, class T>
void gsml::QuadTre<ID, T>::traverse_all(std::vector<T> &all_objects)
{
    for (auto it = m_objects.begin(); it != m_objects.end(); it++)
        all_objects.push_back(it->second);

    if (m_sv) m_sv->traverse_all(all_objects);
    if (m_so) m_so->traverse_all(all_objects);
    if (m_no) m_no->traverse_all(all_objects);
    if (m_nv) m_nv->traverse_all(all_objects);
}


#endif // TQUADTRE_H
