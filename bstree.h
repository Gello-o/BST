#ifndef BSTREE_H
#define BSTREE_H

#include <algorithm>  // std::swap
#include <iostream>
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/**
	@file bstree.h
	@brief Dichiarazione della classe templata bstree
*/


/**
	Classe eccezione custom che deriva da std::logic_error
	Viene generata quando si tenta di creare un sottoalbero a partire da un nodo inesistente

	@brief element not found exception
*/
class element_not_found_exception : public std::logic_error {
public:
	/**
		Costruttore di default 
	*/
    element_not_found_exception() : std::logic_error("Element not found.") {}
};


/**
	Classe eccezione custom che deriva da std::logic_error
	Viene generata quando si tenta di recuperare il successore del valore massimo oppure il predecessore del valore minimo.

	@brief limit value exception
*/
class limit_value_exception : public std::logic_error {
public:
	/**
		Costruttore di default 
	*/
    limit_value_exception() : std::logic_error("Unable to get from a limit value.") {}
};

/**
	Classe che implementa un albero binario di ricerca di dati generici T. 
	L'oridnamento e' effettuati utilizzando un funtore di comparazione C.
	Di default il funtore usa l'orinamento naturale dei dati indotto dagli operatori di confronto standard.

	@brief Albero binario di ricerca

	@param T tipo del dato
	@param C funtore di comparazione (<) di due dati
	@param E funtore di comparazione (==) di due dati
*/

template <typename T, typename C, typename E>
class bstree {

	/**
		Struttura di supporto interna che implementa un nodo dell'albero.

		@brief Nodo dell'albero.
	*/
    
    struct node{
        T value;
        node *left; // puntatore al nodo sinistro dell'albero.
        node *right; // puntatore al nodo destro dell'albero.
        node *p; // puntatore al nodo genitore dell'albero.
        node *next; // puntatore al nodo successivo dell'albero. Per successivivo si intende per ordine d'inserimento.
        
		/**
			Costruttore di default.
		*/
        node() : left(nullptr), right(nullptr), p(nullptr), next(nullptr){ } 
        
		/**
			Costruttore secondario che inizializza il nodo.
			@param v valore del dato
		*/
		node(const T &v) 
			: value(v) , left(nullptr), right(nullptr), p(nullptr), next(nullptr) { } 
        
		

		/**
			Distruttore
		*/
		~node() {
			left = nullptr;
            right = nullptr;
            p = nullptr;
            next = nullptr;
        }
    }; // struct nodo


	node *_root; // Puntatore alla radice dell'albero.
	unsigned int _size;	// Numero di nodi nell'albero.

	C _conf; // Funtore per l'ordinamento.
    E _equal; // Funtore per l'uguaglianza.
    
    /**
		Costruttore che permette di creare

		@param n nodo da cui creare l'albero. 
	*/
    bstree(node *n) : _root(nullptr), _size(0) {
        copy_helper(n); 
    }

	/**
		Funzione helper per copiare un albero in un altro

		@param n nodo da cui partire per copiare l'albero
		@throw eccezione di allocazione di memoria
	*/
    void copy_helper(node *n) {

		try {
            if(n) {
                insert(n->value);
                copy_helper(n->right);
                copy_helper(n->left);
            }
		}
		catch(...) {
			clear();
			throw;
		}
    }
    
    /**
		Funzione helper per la rimozione ricorsiva dei nodi.

		@param n nodo da rimuovere 
	*/
	void clear_helper(node *n) {
		if(n) {
			clear_helper(n->left);
			clear_helper(n->right);
            clear_helper(n->next);
			delete n;
			n = nullptr;	
		}
	}
    
    /**
		Funzione helper per determinare l'esistenza di un nodo nell'albero. 
        L'uguaglianza è definita dal funtore di confronto.

		@param n nodo da confrontare.
		@param value valore da cercare.

		@return TRUE se esiste l'elemento.
	*/
    bool search_helper(node *n, const T &value) const { 
    if (!n) //m==nullptr
        return false; 
  
    if (_equal(n->value, value))
        return true;
  
    bool l = search_helper(n->left, value);
    bool r = search_helper(n->right, value);
        
    return (l || r); 
    }
    
    /**
		Funzione helper per stampare i nodi dell'albero secondo l'attraversamento inorder.

		@param n nodo di ricorsione per l'attraversamento.
	*/
    void print_inorder_helper(node *n) const {
        if (!n)
            return;
        print_inorder_helper(n->left);
        std::cout << n->value << " ";
        print_inorder_helper(n->right);
    }
    
    /**
		Funzione helper per stampare i nodi dell'albero secondo l'attraversamento preorder.

		@param n nodo di ricorsione per l'attraversamento.
	*/
    void print_preorder_helper(node *n) const {
        if (!n)
            return;
        std::cout << n->value << " ";
        print_preorder_helper(n->left);
        print_preorder_helper(n->right);
    }
    
    /**
		Funzione helper per stampare i nodi dell'albero secondo l'attraversamento postorder.

		@param n nodo di ricorsione per l'attraversamento.
	*/
    void print_postorder_helper(node *n) const {
        if (!n)
            return;
        print_postorder_helper(n->left);
        print_postorder_helper(n->right);
        std::cout << n->value << " ";
    }
    
    /**
		Funzione helper per determinare il valore massimo in un albero.

		@param n nodo di ricorsione.
	*/
    T getMax_helper(node *n) const {
        while(n->right) {
            n = n->right;
        }
        return n->value;
    }
    
    /**
		Funzione helper per determinare il valore minimo in un albero.

		@param n nodo di ricorsione.
	*/
    T getMin_helper(node *n) const {
        while(n->left) {
            n = n->left;
        }
        return n->value;
    }
    
    /**
		Funzione helper per determinare successore in un albero.

		@param n nodo da cui determinare il successore.
		@throw limit_value_exception().
	*/
    T successor_helper(node *n) const {
        if(_equal(getMax_helper(_root), n->value)) {
            std::cerr << "No Successor." << std::endl;
            throw limit_value_exception();
        }
        
        node *curr = n;
        
        if(curr->right) {
            return getMin_helper(curr->right);
        } 
        node *prev = curr->p;
        while(prev && (curr==prev->right)) {
            curr = prev;
            prev = prev->p;
        }
        return prev->value;
    }
    
    /**
		Funzione helper per determinare predecessore in un albero.

		@param n nodo da cui determinare il predecessore.
		@throw limit_value_exception().
	*/
    T predecessor_helper(node *n) const {
        if(_equal(getMin_helper(_root), n->value)) {
            std::cerr << "No Predecessor." << std::endl;
            throw limit_value_exception();
        }
        
        node *curr = n;
        
        if(curr->left) {
            return getMax_helper(curr->left);
        } 
        node *prev = curr->p;
        while(prev && (curr==prev->left)) {
            curr = prev;
            prev = prev->p;
        }
        return prev->value;
    }
    
    /**
		Funzione che inserisce un valore in funzione al puntatore next.

		@param value valore da inserire nella lista.
		@throw eccezione di allocazione di memoria.
	*/
    void insert_next(const T& value) {
        if(search_next(value)) return;
        node *tmp;
        
        try {
            tmp = new node(value);
        }
        catch (...) {
            throw;
        }
        
        if(!_root) {
            _root = tmp;
        }
        
        node *curr = _root;
        
        while(curr->next) {
            curr = curr->next;
        }
        curr->next = tmp;
	}
    
    /**
		Funzione per determinare l'esistenza di un nodo nella lista, sempre in funzione al puntatore next. 
        L'uguaglianza è definita dal funtore di confronto.
        
		@param value valore da cercare.

		@return TRUE se esiste l'elemento.
	*/
    bool search_next(const T& value) const {
        return search_next_helper(_root, value);
    }
    
    /**
		Funzione helper per determinare l'esistenza di un nodo nella lista in funzione al puntatore next. 
        L'uguaglianza è definita dal funtore di confronto.

		@param n nodo da confrontare.
		@param value valore da cercare.

		@return TRUE se esiste l'elemento.
	*/
    bool search_next_helper(node *n, const T& value) const {
        if (!n) 
        return false; 

        if (_equal(n->value, value))
            return true; 

        bool l = search_next_helper(n->next, value); 

        return l; 
    }

public:

	/**
		Costruttore di default
	*/
	bstree() : _root(nullptr), _size(0) { }

	/**
		Costruttore di copia

		@param other albero da copiare
		@throw eccezione di allocazione di memoria
	*/
	bstree(const bstree &other) : _root(nullptr), _size(0) {
        copy_helper(other._root);
	}

	/**
		Operatore di assegnamento
		
		@param other albero da copiare
		@throw eccezione di allocazione di memoria
		@return reference a this
	*/
	bstree &operator=(const bstree &other) {
		if(this != &other) {
			bstree tmp(other);
			std::swap(_root,tmp._root);
			std::swap(_size,tmp._size);
		}
		return *this;
	}

	/**
		Distruttore
	*/
	~bstree() {
		clear();
	}

	/**
		Inserisce un elemento in un albero binario di ricerca. Se l'elemento fosse già presente nell'albero, questo non verrà inserito.

		@param value valore da inserire
		@throw eccezione di allocazione di memoria
	*/
    void insert(const T &value) { 
        if(search(value)) return;
        node *tmp;
        
        try {
            tmp = new node(value);
        }
        catch(...) {
            throw;
        }
        
        node *curr = _root;
        node *pred = nullptr;
        while(curr) {
            pred = curr;
            if (_conf(tmp->value, curr->value)) {
                curr = pred->left;
            }
            else {
                curr = curr->right;
            }
        }
        tmp->p = pred;
        
        if(!pred) _root = tmp;
        else if(_conf(tmp->value, pred->value)) {
            pred->left = tmp;
        }
        else {
            pred->right = tmp;
        }
        insert_next(value);
        
        _size++;
    }

	/**
		Cancella i nodi dall'albero
	*/
	void clear() {
		clear_helper(_root);
		_root = nullptr;
        _size = 0;
	}

	/**
		Ritorna il numero di elementi nell'albero

		@return numero di elementi inseriti
	*/
	unsigned int size() const {
		return _size;
	}

	/**
		Determina se esiste un elemento nella lista. L'uguaglianza e' definita dal funtore di confronto

		@param value valore da cercare

		@return TRUE se esiste l'elemento
	*/
    bool search(const T &value) const {
        return search_helper(_root, value);
    }
    
    /**
		Funzione per stampare i nodi dell'albero secondo l'attraversamento inorder.
	*/
    void print_inorder() const {
        print_inorder_helper(_root);
    }
    
    /**
		Funzione per stampare i nodi dell'albero secondo l'attraversamento preorder.
	*/
    void print_preorder() const {
        print_preorder_helper(_root);
    }
    
    /**
		Funzione per stampare i nodi dell'albero secondo l'attraversamento postorder
	*/
    void print_postorder() const {
        print_postorder_helper(_root);
    }
    
    /**
		Funzione  per determinare il valore massimo in un albero
	*/
    T getMax() const {
        getMax_helper(_root);
    }
    
    /**
		Funzione  per determinare il valore minimo in un albero
	*/
    T getMin() const {
        getMin_helper(_root);
    }
    
    /**
		Funzione per determinare successore in un albero.

		@param value valore del nodo di cui cercare il successore.
	*/
    T successor(const T &value) const {
        node *curr = _root;
        
        while(curr) {
            if(_equal(curr->value, value))
                return successor_helper(curr);
            else if(_conf(value, curr->value))
                curr = curr->left;
            else
                curr = curr->right;
        }
    }
    
    /**
		Funzione per determinare successore in un albero.

		@param value valore del nodo di cui cercare il predecessore.
	*/
    T predecessor(const T &value) const {
        node *curr = _root;
        
        while(curr) {
            if(_equal(curr->value, value))
                return predecessor_helper(curr);
            else if(_conf(value, curr->value))
                curr = curr->left;
            else
                curr = curr->right;
        }
    }
    
    /**
		Funzione che crea un sottoalbero a partire dal valore di un nodo. Se l'elemento da cui creare il nuovo sottoalbero non esiste, viene lanciata un'eccezione.
        
        @param value valore del nodo da cui creare il sottoalbero.
        @throw element_not_found_exception.
	*/
    bstree subtree(const T &value) {
        if(!search(value))
            throw element_not_found_exception();
        
        node *curr = _root;
        
        while(curr) {
            if(_equal(curr->value, value)) {
                bstree bst(curr);
                return bst;
            }
            
            if(_conf(value,curr->value)) {
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }
        }
    }

	/**
		Costruttore secondario che costruisce l'albero a partire da una sequenza generica di dati identificata da due iteratori.

		@param begin iteratore di inizio della sequenza
		@param end iteratore di fine della sequenza

		@throw eccezione di allocazione di memoria
	*/
	template <typename IterT>
	bstree(IterT begin, IterT end) 
		: _root(nullptr), _size(0) {

		try {
			while(begin != end) {
				insert(static_cast<T>(*begin));
				++begin;
			}
		}
		catch(...) {
			clear();
			throw;
		}
	}

	/**
		Iteratore costante dell'albero

		@brief Iteratore costante dell'albero
	*/
	class const_iterator {
		const node *_n;

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	
		const_iterator() : _n(nullptr) {
		}
		
		const_iterator(const const_iterator &other) : _n(other._n) {
		}

		const_iterator& operator=(const const_iterator &other) {
			_n = other._n;
			return *this;
		}

		~const_iterator() {
		}

		// Ritorna il dato riferito dall'iteratore (dereferenziamento)
		reference operator*() const {
			return _n->value;
		}

		// Ritorna il puntatore al dato riferito dall'iteratore
		pointer operator->() const {
			return &(_n->value);
		}
        
        const_iterator& operator++() {
            _n = _n->next;
            return *this;
        }

        const_iterator operator++(int) {
			const_iterator tmp(*this);
			_n = _n->next;
			return tmp;
        }
		// Uguaglianza
		bool operator==(const const_iterator &other) const {
			return (_n == other._n);
		}
		
		// Diversita'
		bool operator!=(const const_iterator &other) const {
			return (_n != other._n);
		}

	private:

		// Classe container friend Per usare il costruttore di inizializzazione.
		friend class bstree; 

		// Costruttore privato di inizializzazione usato dalla classe container (tipicamente nei metodi begin e end)
		const_iterator(const node *n) : _n(n) { }
        
	}; // classe const_iterator
	

	/**
		Ritorna l'iteratore all'inizio della sequenza dati
	
		@return iteratore all'inizio della sequenza
	*/
	const_iterator begin() const {
		return const_iterator(_root);
	}
	
	/**
		Ritorna l'iteratore alla fine della sequenza dati
	
		@return iteratore alla fine della sequenza
	*/
	const_iterator end() const {
		return const_iterator(nullptr);
	}
    
};

/**
	Ridefinizione dell'operatore di stream per la stampa del contenuto dell'albero

	@param os oggetto stream di output
	@param bst albero da stampare

	@return reference allo stream di output
*/
template <typename T, typename C, typename E>
std::ostream &operator<<(std::ostream &os, 
	const bstree<T,C,E> &bst) {
	
	typename bstree<T,C,E>::const_iterator i,ie;
	
	i = bst.begin();
	ie = bst.end();

	while(i!=ie) {
		os << *i << " ";
		++i;
	}

	return os;
}

/**
    @brief stampa di elementi che soddisfano il predicato


	Stampa gli elementi che soddisfano il predicato

	@param bst albero su cui verificare il predicato
	@param pred predicato da soddisfare
*/
template <typename T, typename C, typename E, typename P>
void printif(const bstree<T,C,E> &bst, P pred) {
	
	typename bstree<T,C,E>::const_iterator i,ie;

	i = bst.begin();
	ie = bst.end();
	
	while(i!=ie) {
		if(pred(*i)) {
			std::cout << *i << " ";
		}
		++i;
	}
}

#endif