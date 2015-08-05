#pragma once

#include "dukbindconf.h"

#if DUKBIND_DEBUG
    #include <string>
#endif

namespace dukbind
{
    namespace hash
    {
        template< int CharacterCount, int Index = CharacterCount - 1>
        struct Generator
        {
            inline static unsigned int Hash( const char ( &character_array )[ CharacterCount ] )
            {
                return ( Generator<CharacterCount, Index-1>::Hash( character_array ) *16777619u ) ^ character_array[ Index-1 ];
            }
        };

        template< int CharacterCount>
        struct Generator<CharacterCount, 0>
        {
            inline static unsigned int Hash( const char ( & )[ CharacterCount ] )
            {
                return 2166136261u;
            }
        };

        template<>
        struct Generator<1, 0>
        {
            inline static unsigned int Hash( const char ( & )[ 1 ] )
            {
                return 0u;
            }
        };

        template< int CharacterCount >
        unsigned int GetHashCodeCT( const char (& text)[ CharacterCount ] )
        {
            return Generator<CharacterCount>::Hash( text );
        }

        unsigned int GetHashCode(
            const char * text
            );
    }

    class Identifier
    {

    public :

        Identifier();

        #if !DUKBIND_DEBUG
            template< int ItemCount >
            inline Identifier(
                const char ( &text )[ ItemCount ]
                ) :
                HashCode( hash::Generator<ItemCount>::Hash( text ) )
            {
            }
        #endif

        explicit Identifier( const char * text ) :
            HashCode()
            #if DUKBIND_DEBUG
                , Text()
            #endif
        {
            Initialize( *this, text );
        }

        Identifier( const Identifier & other ) = default;
        Identifier( const unsigned int check_sum );
        ~Identifier();

        Identifier & operator =( const Identifier & other ) = default;
        Identifier & operator =( const unsigned int & check_sum );

        bool operator ==( const Identifier & other ) const;
        bool operator !=( const Identifier & other ) const;

        #if DUKBIND_DEBUG
            const std::string & GetText() const
            {
                return Text;
            }
        #endif

        unsigned int GetHashCode() const
        {
            return HashCode;
        }

        bool IsEmpty() const
        {
            return HashCode == 0;
        }

    private :

        static void Initialize(
            Identifier & identifier,
            const char * text
            );

        // .. ATTRIBUTES

        unsigned int
            HashCode;
        #if DUKBIND_DEBUG
            std::string
                Text;
        #endif
    };

    inline Identifier & Identifier::operator =( const unsigned int & check_sum )
    {
        HashCode = check_sum;

        #if DUKBIND_DEBUG
            Text = "";
        #endif

        return *this;
    }

    inline bool Identifier::operator ==( const Identifier & other ) const
    {
        return HashCode == other.HashCode;
    }

    inline bool Identifier::operator !=( const Identifier & other ) const
    {
        return HashCode != other.HashCode;
    }

    inline bool operator <( const Identifier & first, const Identifier & second )
    {
        return first.GetHashCode() < second.GetHashCode();
    }
}

