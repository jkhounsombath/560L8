#include <iostream>

template <typename Comparable>
class myLeftistHeap
{
    public:
        myLeftistHeap( )
        {
            root = nullptr;
        }

        myLeftistHeap( const myLeftistHeap & rhs )
        {
            root = clone(rhs.root);
        }

        myLeftistHeap( myLeftistHeap && rhs )
        {
            root = std::move(rhs.root);
        }

        ~myLeftistHeap( )
        {
            makeEmpty();
        }

        myLeftistHeap & operator=( const myLeftistHeap & rhs )
        {
            makeEmpty();
            root =  clone(rhs.root);
            return *this;
        }

        myLeftistHeap & operator=( myLeftistHeap && rhs )
        {
            root = std::move(rhs.root);
            return *this;
        }

        bool isEmpty( ) const
        {
            return root == nullptr;
        }

        const Comparable & findMin( ) const
        {
            return root->element;
            
        }

        void insert( const Comparable & x )
        {
            root = merge( new LeftistNode{ x }, root );
        }

        void insert( Comparable && x )
        {
            root = std::move(merge(new LeftistNode{x}, root));
        }

        void deleteMin( )
        {
            if( isEmpty( ) )
                std::cout << "underflow exception\n";

            LeftistNode *oldRoot = root;
            root = merge( root->left, root->right );
            delete oldRoot;
        }

        void deleteMin( Comparable & minItem )
        {
            minItem = findMin( );
            deleteMin( );
        }

        void makeEmpty( )
        {
            while(isEmpty() == false)
            {
                deleteMin();
            }
        }

        void merge( myLeftistHeap & rhs )
        {
            if( this == &rhs ) // Avoid aliasing problems
                return;

            root = merge( root, rhs.root );
            rhs.root = nullptr;
        }

        void printInOrder()
        {
            printInOrderRec(root);
        }

        void printLevelOrder()
        {
            if(isEmpty() == false)
            {
                int depth = getDepth(root);
                for(int i = 0; i <= depth; i++)
                    printLevelOrderRec(root, i);
            }
        }

    private:
        struct LeftistNode
        {
            Comparable element;
            LeftistNode *left;
            LeftistNode *right;
            int npl;

            LeftistNode( const Comparable & e, LeftistNode *lt = nullptr,
            LeftistNode *rt = nullptr, int np=0)
            : element{ e }, left{ lt }, right{ rt }, npl{ np}{}

            LeftistNode( Comparable && e, LeftistNode *lt = nullptr,
            LeftistNode *rt = nullptr, int np=0)
            : element{ std::move( e ) }, left{ lt }, right{ rt }, npl{ np}{}
        };

        LeftistNode *root;

        LeftistNode * merge( LeftistNode *h1, LeftistNode *h2 )
        {
            if( h1 == nullptr )
                return h2;
            if( h2 == nullptr )
                return h1;
            if( h1->element < h2->element )
                return merge1( h1, h2 );
            else
                return merge1( h2, h1 );
        }

        LeftistNode * merge1( LeftistNode *h1, LeftistNode *h2 )
        {
            if( h1->left == nullptr ) // Single node
                h1->left = h2; // Other fields in h1 already accurate
            else
            {
                h1->right = merge( h1->right, h2 );
                if( h1->left->npl < h1->right->npl )
                    swapChildren( h1 );
                h1->npl = h1->right->npl + 1;
            }
            return h1;
        }

        void swapChildren( LeftistNode *t )
        {
            LeftistNode * s = t->left;
            t->left = t->right;
            t->right = s;
        }

        LeftistNode * clone( LeftistNode *t ) const
        {
            if(t == nullptr)
            {
                return nullptr;
            }
            else
            {
                return new LeftistNode(t->element, clone(t->left), clone(t->right),t->npl);
            }
            
        }

        void printInOrderRec(LeftistNode *curNode)
        {
            if(curNode == nullptr)
                return;
            else
            {
                printInOrderRec(curNode->left);
                std::cout << curNode->element << std::endl;
                printInOrderRec(curNode->right);
            }
        }

        void printLevelOrderRec(LeftistNode *curNode, int depth)
        {
            if(curNode == nullptr)
                return;
            if (depth == 1)
            {
                std::cout<< curNode->element <<std::endl;
            }
            else if (depth > 1)
            {
                printLevelOrderRec(curNode->left, depth - 1);
                printLevelOrderRec(curNode->right, depth -1);
            }
        }

        int getDepth(LeftistNode *t)
        {
            int lDepth = getDepth(t->left);
            int rDepth = getDepth(t->right);
            if(lDepth > rDepth)
                return (lDepth+1);
            else
                return rDepth+1;      
        }
};