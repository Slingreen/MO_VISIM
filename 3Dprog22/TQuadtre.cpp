//#include "TQuadtre.h"

//template<class ID, class T>
//gsml::QuadTre<ID, T>::QuadTre()
//{
//    m_a = Point2D(-1,-1);
//    m_b = Point2D(1,-1);
//    m_c = Point2D(1,1);
//    m_d = Point2D(-1,1);

//    m_sv = m_so = m_no = m_nv = nullptr;
//}

//template<class ID, class T>
//gsml::QuadTre<ID, T>::QuadTre(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4)
//    :m_a{v1}, m_b{v2}, m_c{v3}, m_d{v4},
//      m_sv{nullptr}, m_so{nullptr}, m_no{nullptr}, m_nv{nullptr}
//{
//    //
//}

//template<class ID, class T>
//void gsml::QuadTre<ID, T>::init(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4)
//{
//    m_a = v1; m_b = v2; m_c = v3; m_d = v4;
//}

//template<class ID, class T>
//void gsml::QuadTre<ID, T>::subDivide(int n)
//{
//    if (n > 0) {

//        Point2D v1 = Point2D((m_a.first + m_b.first)/2, (m_a.second + m_b.second)/2);
//        Point2D v2 = Point2D((m_b.first + m_c.first)/2, (m_b.second + m_c.second)/2);
//        Point2D v3 = Point2D((m_c.first + m_d.first)/2, (m_c.second + m_d.second)/2);
//        Point2D v4 = Point2D((m_d.first + m_a.first)/2, (m_d.second + m_a.second)/2);
//        Point2D m = Point2D((m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2);

//        m_sv = new QuadTre (m_a, v1 , m, v4 ) ;
//        m_sv->subDivide ( n-1);
//        m_so = new QuadTre ( v1 , m_b, v2 , m) ;
//        m_so->subDivide ( n-1);
//        m_no = new QuadTre (m, v2 , m_c, v3 ) ;
//        m_no->subDivide ( n-1);
//        m_nv = new QuadTre ( v4 , m, v3 , m_d) ;
//        m_nv->subDivide ( n-1);
//    }

//}

//template<class ID, class T>
//void gsml::QuadTre<ID, T>::print() const
//{
//    Point2D m = Point2D((m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2);

//    std::cout << "senter = (" << m.first << ", " << m.second << ")" << std::endl;
//    for (auto it = m_objects.begin(); it != m_objects.end(); it++)
//        std::cout << it->first << ", " << it->second->m_navn << std::endl;

//}

//template<class ID, class T>
//void gsml::QuadTre<ID, T>::print_all() const
//{
//    print();
//    if (m_sv) m_sv->print_all();
//    if (m_sv) m_so->print_all();
//    if (m_sv) m_no->print_all();
//    if (m_sv) m_nv->print_all();
//}

//template<class ID, class T>
//gsml::QuadTre<ID, T> *gsml::QuadTre<ID, T>::insert(const Point2D &point, const ID &id, const T &object)
//{
//    if (isLeaf())
//    {
//        m_objects.push_back(std::pair<ID,T>{id, object});
//        return this;
//    }
//    else
//    {
//        Point2D m = Point2D{(m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2};

//        if (point.second < m.second)
//        {
//            if (point.first < m.first)
//                m_sv->insert(point, id, object);
//            else
//                m_so->insert(point, id, object);
//        }
//        else
//        {
//            if (point.first < m.first)
//                m_nv->insert(point, id, object);
//            else
//                m_no->insert(point, id, object);
//        }
//    }
//}

//template<class ID, class T>
//gsml::QuadTre<ID, T> *gsml::QuadTre<ID, T>::find(const Point2D &p)
//{
//    if (isLeaf())
//        return this;
//    else
//    {
//        Point2D m = Point2D{(m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2};

//        if (p.second < m.second)
//        {
//            if (p.first < m.first)
//                m_sv->find(p);
//            else
//                m_so->find(p);
//        }
//        else
//        {
//            if (p.first < m.first)
//                m_nv->find(p);
//            else
//                m_no->find(p);
//        }
//    }
//}

//template<class ID, class T>
//const gsml::QuadTre<ID, T> *gsml::QuadTre<ID, T>::find(const Point2D &p) const
//{
//    // samme som uten const??
//    if (isLeaf())
//        return this;
//    else
//    {
//        Point2D m = Point2D{(m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2};

//        if (p.second < m.second)
//        {
//            if (p.first < m.first)
//                m_sv->find(p);
//            else
//                m_so->find(p);
//        }
//        else
//        {
//            if (p.first < m.first)
//                m_nv->find(p);
//            else
//                m_no->find(p);
//        }
//    }
//}

//template<class ID, class T>
//T &gsml::QuadTre<ID, T>::find(const ID &id)
//{
//    if (isLeaf() && m_objects.size()>0)
//    {
//        for (auto it = m_objects.begin(); it != m_objects.end(); it++)
//        {
//            if (it->first == id)
//                return it->second;
//        }
//    }
//    else if (m_sv)
//            m_sv->find(id);
//    else if (m_so)
//            m_so->find(id);
//    else if (m_no)
//            m_no->find(id);
//    else if (m_nv)
//            m_nv->find(id);
//}

//template<class ID, class T>
//T &gsml::QuadTre<ID, T>::operator[](ID id)
//{
//    if (isLeaf())
//    {
//        for (auto it = m_objects.begin(); it != m_objects.end(); it++)
//        {
//            if (it->first == id)
//                return it->second;
//        }
//    }
//}

//template<class ID, class T>
//bool gsml::QuadTre<ID, T>::isLeaf() const
//{
//    return m_sv == nullptr && m_so == nullptr && m_no == nullptr && m_nv == nullptr;
//}

//template<class ID, class T>
//void gsml::QuadTre<ID, T>::traverse_all(std::vector<T> &all_objects)
//{
//    for (auto it = m_objects.begin(); it != m_objects.end(); it++)
//        all_objects.push_back(it->second);

//    if (m_sv) m_sv->traverse_all(all_objects);
//    if (m_so) m_so->traverse_all(all_objects);
//    if (m_no) m_no->traverse_all(all_objects);
//    if (m_nv) m_nv->traverse_all(all_objects);
//}
