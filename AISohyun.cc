#include "Player.hh"
#include <list>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Sohyun


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  const vector<Dir> dirs_mag = {Up,Down,Left,Right};
  const vector<Dir> dirs_fant = {Up,Down,Left,Right,RU,UL,DR,LD};

  // Retorna solució vector de Dumbledore
  // Pre: El vector es pot agrupar en grups de mida 3 de manera que cada grup sumi la mateixa quantitat
  // Post: Retorna a les posicions on es troba cada valor el seu grup. No sempre troba solució.
  const vector<int> arcane () {
    // Les instruccions cout comentades eran per fer debugging
    vector<int> v = spell_ingredients(); //V es el vector de Dumbledore
    int n = v.size(); 
    vector<int> sol(n,-1); //Vector solució
    vector<list<int>> x(31); //Vector de listes amb posició de 0 a 30
    int s = 0;
    for (int i = 0; i<n; ++i)  {
      s+=v[i];  //Calculem suma a s
      x[v[i]].push_back(i); //Guardem a la posició del valor del vector de llistes, a la llista, la posició on es troba
    }
    //cout << s << " "; //Comprobar suma total
    s = s/(n/3);  //Suma que ha de tenir el subvector
    //cout << s << endl; //Comprobar suma dels subvectors
    int p = 0; //Grup inicial
    
      
      for (int i = 0; i<31; ++i) {  
        while (not x[i].empty()) {  //Si existeix algun numero i
          /*cout << "iteracion en " << i << endl;
          for(int m = 0; m<31; ++m) {
            cout << m << ": ";
            for (list<int>::iterator it = x[m].begin(); it!=x[m].end();++it) {
              cout << *it << " ";
            }
            cout << endl;
          }*/
          int j = i;
          if (x[i].size()==1) ++j; //Si nomes hi ha un i, j no pot ser i
          while(x[j].empty()) ++j; //Busquem un numero fins trobar un valor de j
          int y = s - (v[x[i].back()]+v[x[j].back()]); // Valor per que s = i+j+y
          if (i==j and j==y and x[i].size()<3) { //Si els 3 valen el mateix pero no hi ha 3 numeros iguals
            ++j;
            y = s - (v[x[i].back()]+v[x[j].back()]); 
          }
          while (j!=31 and x[y].empty()) {
            ++j;
            while(x[j].empty()) ++j;
            if (j!=31) y = s - (v[x[i].back()]+v[x[j].back()]);
            if (j==y and x[j].size()<2) { //Si j i y valen el mateix pero no hi ha 2 numros iguals
              ++j;
              while(x[j].empty()) ++j;
              y = s - (v[x[i].back()]+v[x[j].back()]);
            }
          }
          if (j!=31) {
          //cout << "i j y: " << i << " " << j << " " << y  << endl; //comprobar els valorss
          sol[x[i].back()] = p; //Afegim a la solució i eliminem de la llista
          x[i].pop_back();
          sol[x[j].back()] = p;
          x[j].pop_back();
          sol[x[y].back()] = p;
          x[y].pop_back();
          ++p;
          //for (int i = 0; i<v.size(); ++i) cout << sol[i] << " "; //veure com esta el vector despres de modificar
          //cout << endl; 
          }
          else return sol; //el nostre algoritme no troba solució
          }
          //cout << i << ": ";
          //for (int i = 0; i<v.size(); ++i) cout << sol[i] << " "; //veure com esta el vector cada iteració
          //cout << endl;        
        }
    return sol;
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
        vector<int> ws = wizards(me());
        
        
        int g = ghost(me());
        int rondes_per_atac = unit(g).resting_rounds();
        if (rondes_per_atac == 0) spell(g,arcane());

  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
