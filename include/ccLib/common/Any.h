#ifndef ANY_H
#define ANY_H

#include <typeinfo>
#include <type_traits>

namespace CCPlayer
{

class Any
{
template<typename ValueType>
friend ValueType * any_cast(Any *);

template<typename ValueType>
friend ValueType * any_cast(const CCPlayer::Any * operand);

template<typename ValueType>
ValueType any_cast(CCPlayer::Any & operand);

template<typename ValueType>
const ValueType any_cast(const CCPlayer::Any & operand);

public:
    Any():content(0)
    {
    }

    template<typename ValueType>
    Any(const ValueType & value): content(new holder<ValueType>(value))
    {
    }

    Any(const Any & other): content(other.content ? other.content->clone() : 0)
    {
    }

    ~Any()
    {
        delete content;
    }
    Any& swap(Any & rhs)
    {
        std::swap(content, rhs.content);
        return *this;
    }

    template<typename ValueType>
    Any & operator=(const ValueType & rhs)
    {
        Any(rhs).swap(*this);
        return *this;
    }

    Any & operator=(const Any & rhs)
    {
        Any(rhs).swap(*this);
        return *this;
    }

    bool empty() const
    {
        return !content;
    }

    const std::type_info & type() const
    {
        return content ? content->type() : typeid(void);
    }
private:
    class placeholder
    {
    public:
        virtual ~placeholder()
        {
        }
        virtual const std::type_info & type() const = 0;
        virtual placeholder * clone() const = 0;
    };

    template<typename ValueType>
    class holder : public placeholder
    {
    public:
        holder(const ValueType & value): held(value)
        {
        }
        virtual const std::type_info & type() const
        {
            return typeid(ValueType);
        }

        virtual placeholder * clone() const
        {
            return new holder(held);
        }

        ValueType held;
    };
private:
    //the only member data
    placeholder* content;
};

class bad_any_cast : public std::bad_cast
{
public:
    virtual const char * what() const throw()
    {
        return "boost::bad_any_cast: failed conversion using boost::any_cast";
    }
};

template<typename ValueType>
ValueType* any_cast(CCPlayer::Any * operand)
{
    return ( operand && operand->type() == typeid(ValueType) )
            ? &static_cast<CCPlayer::Any::holder<ValueType> *>(operand->content)->held
            : 0;
}

template<typename ValueType>
const ValueType * any_cast(const CCPlayer::Any * operand)
{
    return any_cast<ValueType>(const_cast<CCPlayer::Any *>(operand));
}

template<typename ValueType>
const ValueType any_cast(const CCPlayer::Any & operand)
{
    typedef typename std::remove_reference<ValueType>::type nonref;
    const nonref * result = any_cast<nonref>(&operand);
    if(!result)
            throw bad_any_cast();
    return *result;
}

template<typename ValueType>
ValueType any_cast(CCPlayer::Any & operand)
{
    typedef typename std::remove_reference<ValueType>::type nonref;
    nonref * result = any_cast<nonref>(&operand);
    if(!result)
            throw bad_any_cast();
    return *result;
}

}
#endif
