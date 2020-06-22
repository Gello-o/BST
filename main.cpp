#include <iostream>
#include "bstree.h"
#include <cassert> // assert


/**
	Funtore per il confronto tra numeri interi. 
    Ritorna TRUE se il primo numero è minore del secondo

	@brief Funtore per il confronto tra numeri interi.
*/
struct compare_int {
	bool operator()(int a, int b) const {
		return a<b;
	}
};

/**
	Funtore per l'uguaglianza tra numeri interi.. 

	@brief Funtore per il confronto tra numeri interi.
*/
struct equal_int {
	bool operator()(int a, int b) const {
		return a==b;
	}
};


/**
	Funtore per il confronto tra stringhe.
    Ritrona TRUE se la prima stringa è più piccola della seconda.

	@brief Funtore per il confronto tra stringhe.
*/
struct compare_string {
	bool operator()(const std::string &a, const std::string &b) const {
		return (a.length()<b.length());
	} 
};

/**
	Funtore per l'uguaglianza tra stringhe. 

	@brief Funtore per il confronto tra stringhe.
*/
struct equal_string {
	bool operator()(const std::string &a, const std::string &b) const {
		return (a==b);
	} 
};


/**
	Predicato utilizzato per la funzione globale printif.
    Ritorna TRUE se il valore a intero è minore di 20 e maggiore di 10.

	@brief Funtore per il confronto di due punti.
*/
struct ss {
	bool operator()(int a) const {
		return ((a<20) && (a>10));
	}
};

/**
	Struct point che implementa un punto 2D.

	@brief Struct point che implementa un punto 2D.
*/
struct point {
	int x; //Cordinata punto x
	int y; //Cordinata punto y

	point(int xx, int yy) : x(xx), y(yy) {}
};

/**
	Funtore per il confronto sull'uguaglianza tra due punti.
    Ritorna TRUE se p1.x < p2.x, nel caso fossero ugali quindi se p1.y < p2.y 

	@brief Funtore per il confronto di due punti.
*/
struct compare_point {
	bool operator()(const point &p1, const point &p2) const {
        if(p1.x==p2.x)
            return (p1.y<p2.y);
		return (p1.x<p2.x);
	}
};

/**
	Funtore per il confronto sull'uguaglianza tra due punti.
    Ritorna TRUE se p1.x = p2.x e p1.y = p2.y

	@brief Funtore per il confronto di due punti.
*/
struct equal_point {
	bool operator()(const point &p1, const point &p2) const {
		return (p1.x==p2.x) && (p1.y==p2.y);
	} 
};

/**
	Ridefinizione dell'operatore di stream << per un point.
	Necessario per l'operatore di stream della classe bstree.
*/
std::ostream &operator<<(std::ostream &os, const point &p) {
	std::cout<<"("<<p.x<<","<<p.y<<")";
	return os;
}



typedef bstree<int,compare_int,equal_int> bstint;

void test_constructor() {
	std::cout << "****** Test costruttori e stampa di valori interi ******" << std::endl;

	bstint bst;

	std::cout<<"Inserimento dei valori 20, 15, 10, 17, 25, 30, 23";
    bst.insert(20);
    bst.insert(15);
    bst.insert(10);
    bst.insert(17);
    bst.insert(25);
    bst.insert(30);
    bst.insert(23);

	std::cout << std::endl << "INORDER PRINT (bst)" << std::endl;
    bst.print_inorder();
    assert(bst.size()==7);
    
    
	bstint bst2(bst);

	std::cout << std::endl << "PREORDER PRINT (bst2)" << std::endl;
    bst2.print_preorder();
    assert(bst2.size()==bst.size());
    
    
	bstint bst3;
	bst3 = bst;	

	std::cout << std::endl << "POSTORDER PRINT (bst3)" << std::endl;
    bst3.print_postorder();
    assert(bst3.size()==bst.size());
};


void test_gestione_albero() {
	std::cout << std::endl << std::endl << "****** Test sulla gestione dell'albero di valori interi ******"<<std::endl;
    bstint bst;

	std::cout<<"Inserimento dei valori 1, 2, 0, 20, -5"<<std::endl;
	bst.insert(1);
	bst.insert(2);
	bst.insert(0);
    bst.insert(20);
    bst.insert(-5);
    
    std::cout << "Stampa valore minimo: " << bst.getMin() << std::endl;
    
    std::cout << "Stampa valore massimo: " << bst.getMax() << std::endl;

	assert(bst.size() == 5);
	std::cout << "Dimensione albero: " << bst.size() << std::endl;	
    
	bool flag1 = bst.search(20);
	assert(flag1 == true);
	std::cout << "Ricerca del nodo con valore 20: " << flag1 << std::endl;

	bool flag2 = bst.search(-20);
	assert(flag2 == false);
	std::cout << "Ricerca del nodo con valore -20: " << flag2 << std::endl;
    
    std::cout << "Successore di -5: " << bst.successor(-5) << std::endl;
    
    std::cout << "Predecessore di 20: " << bst.predecessor(20) << std::endl;

	bstint::const_iterator i,ie;
	std::cout<<"Stampa con iteratori: "<<std::endl;
	for(i=bst.begin(),ie=bst.end(); i!=ie; ++i)
		std::cout << *i << " ";
}

void test_const_bst_int(const bstint &other) {
	std::cout << std::endl << std::endl << "****** Test su un albero costante di interi ******" << std::endl;

	std::cout << "Stampa con operator<<" << std::endl;
	std::cout << other;

	std::cout << "Dimensione dell'albero: " << other.size() << std::endl;

	std::cout << "Stampa con iteratori" << std::endl;

	bstint::const_iterator i,ie;

	for(i=other.begin(),ie=other.end(); i!=ie; i++)
		std::cout << *i << std::endl;
}


void test_predicate() {
	std::cout << std::endl << std::endl << "****** Test sul predicato printif per valori interi ******" << std::endl;
    
    bstint bst; 
    
	std::cout << "Inserimento dei valori 20, 15, 10, 17, 25, 30, 23" << std::endl;
    bst.insert(20);
    bst.insert(15);
    bst.insert(10);
    bst.insert(17);
    bst.insert(25);
    bst.insert(30);
    bst.insert(23);
    
    std::cout << "Stampa di valori (<20 && >10): " << std::endl;
    printif(bst,ss());
}


void test_subtree() {
	std::cout << std::endl << std::endl << "****** Test sull'uso del metodo Subtree su un albero di valori interi ******" << std::endl;
    
    bstint bst; 
    
	std::cout << "Inserimento dei valori 1, 0, 20, 2, -5, 50, 5, -13" << std::endl;
	bst.insert(1);
	bst.insert(0);
    bst.insert(20);
	bst.insert(2);
    bst.insert(-5);
    bst.insert(50);
    bst.insert(5);
    bst.insert(-13);
    
	std::cout << "Crazione sottoalbero con radice 20:" << std::endl;
    bstint bst2 = bst.subtree(20);
    std::cout << "STAMPA CON OPERATOR <<" << std::endl;
    std::cout << bst2 << std::endl;
    std::cout << "Dimensione Sottoalbero: " << bst2.size() << std::endl << std::endl;
}


void test_string() {
	std::cout<<"****** Test su un albero di stringhe ******"<<std::endl;

	bstree<std::string, compare_string, equal_string> bst;

	std::cout<<"Inserimento dei valori 'jimmie', 'mia', 'marsellus', 'jules 'vincent'" << std::endl;
	bst.insert("jimmie");
	bst.insert("mia");
	bst.insert("marsellus");
	bst.insert("jules");
	bst.insert("vincent");

	std::cout << "INORDER PRINT" << std::endl;
    bst.print_inorder();

	std::cout << std::endl << "PREORDER PRINT" << std::endl;
    bst.print_preorder();

	std::cout << std::endl << "POSTORDER PRINT" << std::endl;
    bst.print_postorder();

	std::cout << std::endl << "Dimensione albero: " << bst.size() << std::endl;
    
	std::cout<<"Successore di 'jimmie': "<< bst.successor("jimmie") <<std::endl;
	std::cout<<"Predecessore di 'jimmie': "<< bst.predecessor("jimmie") <<std::endl;
    
	assert(bst.search("vincent") == true);
	std::cout<<"Ricerca di 'vincent': "<< bst.search("vincent") <<std::endl;
    assert(bst.search("butch") == false);
	std::cout<<"Ricerca di 'butch': "<< bst.search("butch") <<std::endl;
}

void test_point() {

	std::cout << std::endl << "****** Test su un albero di valori point ******"<<std::endl;

	bstree<point, compare_point, equal_point> bst;

	std::cout<<"Inserimento dei valori (1,1), (1,2), (2,7), (0,0), (5,4), (2,4), (1,0)"<<std::endl;
	bst.insert(point(1,1));
	bst.insert(point(1,2));
	bst.insert(point(2,7));
	bst.insert(point(0,0));
	bst.insert(point(5,4));
	bst.insert(point(2,4));
	bst.insert(point(1,0));

	std::cout << "Stampa con operator<<" << std::endl;
	std::cout << bst << std::endl;

	std::cout<<"Stampa con iteratori: "<<std::endl;

	bstree<point, compare_point, equal_point>::const_iterator i,ie;

	for(i=bst.begin(),ie=bst.end(); i!=ie; ++i)
		std::cout<<*i<< " ";

	std::cout << std::endl << "Dimensione albero: " << bst.size() << std::endl;
    
	std::cout<<"Massimo: "<< bst.getMax() <<std::endl;
	std::cout<<"Minimo: "<< bst.getMin() <<std::endl;

	std::cout<< "Ricerca di '(1,1)': "<<bst.search(point(1,1))<<std::endl;
	std::cout<<"Ricerca di '(2,2)': "<<bst.search(point(2,2))<<std::endl;
}



int main() {
    const bstint bst;
    
	test_constructor();
    test_gestione_albero();
    test_const_bst_int(bst);
    test_predicate();
    test_subtree();
    test_string();
    test_point();
    
    
	return 0;
}