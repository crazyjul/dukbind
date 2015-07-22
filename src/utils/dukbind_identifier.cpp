#include "dukbindconf.h"
#include "dukbind_identifier.h"
#include "dukbind_assert.h"

#if DUKBIND_DEBUG
    #include <deque>
    #include <string>
    #include <mutex>
#endif

namespace dukbind
{

    unsigned int hash::GetHashCode(
        const char * text
        )
    {
        dukbind_assert( text, "Expected valid text" );
        unsigned int hash_code = 2166136261u;

        while( *text )
        {
            hash_code = ( hash_code * 16777619u ) ^ ( *text++ );
        }

        return hash_code;
    }

    Identifier::Identifier() :
        HashCode( 0 )
        #if DUKBIND_DEBUG
            , Text()
        #endif
    {
    }

    Identifier::Identifier( const unsigned int check_sum ) :
        HashCode( check_sum )
        #if DUKBIND_DEBUG
            , Text()
        #endif
    {
    }

    Identifier::~Identifier()
    {
    }

    void Identifier::Initialize(
        Identifier & identifier,
        const char * text
        )
    {

        if ( !text || !text[ 0 ] )
        {
            identifier.HashCode = 0;
            return;
        }

        identifier.HashCode = hash::GetHashCode( text );

        #if DUKBIND_DEBUG

            identifier.Text = text;

            static std::mutex mtx;
            {
                std::lock_guard<std::mutex> lck (mtx);
                static std::deque< unsigned int > hash_code_table;
                static std::deque< std::string > text_table;

                dukbind_assert( hash_code_table.size() == text_table.size(), "Hash code and text table size should be equal" );

                auto it = std::find( hash_code_table.begin(), hash_code_table.end(), identifier.HashCode );

                if( it != hash_code_table.end() )
                {
                    auto index = it - hash_code_table.begin();

                    dukbind_assert( text_table[ index ] == text, "Hash code collision" );
                }
                else
                {
                    hash_code_table.push_back( identifier.HashCode );
                    text_table.push_back( text );
                }
            }
        #endif
    }
}

