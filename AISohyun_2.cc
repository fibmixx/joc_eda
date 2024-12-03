#include "Player.hh"
#include <list>
#include <vector>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Sohyun_2


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
  typedef vector<int> VI;
  typedef vector<VI> MI;

  // Retorna solució vector de Dumbledore
  // Pre: El vector es pot agrupar en grups de mida 3 de manera que cada grup sumi la mateixa quantitat
  // Post: Retorna a les posicions on es troba cada valor el seu grup. No sempre troba solució.
  const vector<int> arcane () {
    // Les instruccions cout comentades eran per fer debugging en un altre programa
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

  bool bsearch(const vector<int>& A, const int& x, int l, int u) {
    if (l == u + 1) {
      if (A[l]==x or A[u]==x) return true;
      return false;
    }
    int m = (l + u) / 2;
    if (A[m]==x) return true;
    if (x < A[m])
    return bsearch(A, x, l, m);
    else
    return bsearch(A, x, m, u);
  }

  int libro(Pos p, int id, MI mat) {
    queue<Pos> q; 
    q.push(p);
    mat[p.i][p.j] = 0;
    int it = 0;
    while (not q.empty()) {
      p = q.front();
      q.pop();
      int i = p.i;
      int j = p.j;
      if (cell(i,j).book) { 
        //cerr << unit(id).pos.i << " " << unit(id).pos.j << " encuentra en: " << p.i << " " <<p.j << " diferencia de " << p.i-unit(id).pos.i << " " << p.j-unit(id).pos.j <<endl;
        return mat[i][j];
      }
      else {
        for (int k=0; k<4; ++k) {
          Pos pp = p+dirs_mag[k];
          if (pos_ok(pp) and cell(pp).type!=Wall and mat[pp.i][pp.j]==-1) {
            q.push(pp);
            mat[pp.i][pp.j] = 1+mat[i][j];
          }
        }
      }
    }
    return 1000;
  }

  Dir pos(int id) {
    MI mat(60,VI(60,-1));
    VI x(4,10000);
    Pos p = unit(id).pos;
    mat[p.i][p.j]=1;
    for (int k=0; k<4; ++k) {
      Pos pp = p+dirs_mag[k];
      if (pos_ok(pp) and cell(pp).type!=Wall) {
        x[k] = libro(pp,id,mat);
        //cerr << p << dirs_mag[k] << " --- " << x[k] <<endl;
      }
    }
    //for (int i = 0; i<4; ++i) cerr << x[i] << " ";
    //cerr <<endl;
    int min = 1000;
    int posmin = 0;
    for (int i = 0; i<4; ++i) {
      if (x[i]<min)  {
        min = x[i];
        posmin = i;
        }
    }
    //cerr << "distancia " << x[min];
    return dirs_mag[posmin];
  }

    int libro_f(Pos p, int id, MI mat) {
    queue<Pos> q; 
    q.push(p);
    mat[p.i][p.j] = 0;
    int it = 0;
    while (not q.empty()) {
      p = q.front();
      q.pop();
      int i = p.i;
      int j = p.j;
      if (cell(i,j).book) { 
        //cerr << unit(id).pos.i << " " << unit(id).pos.j << " encuentra en: " << p.i << " " <<p.j << " diferencia de " << p.i-unit(id).pos.i << " " << p.j-unit(id).pos.j <<endl;
        return mat[i][j];
      }
      else {
        for (int k=0; k<8; ++k) {
          Pos pp = p+dirs_fant[k];
          if (pos_ok(pp) and cell(pp).type!=Wall and mat[pp.i][pp.j]==-1) {
            q.push(pp);
            mat[pp.i][pp.j] = 1+mat[i][j];
          }
        }
      }
    }
    return 1000;
  }

  Dir pos_f(int id) {
    MI mat(60,VI(60,-1));
    VI x(8,10000);
    Pos p = unit(id).pos;
    mat[p.i][p.j]=1;
    for (int k=0; k<8; ++k) {
      Pos pp = p+dirs_fant[k];
      if (pos_ok(pp) and cell(pp).type!=Wall) {
        x[k] = libro(pp,id,mat);
        //cerr << p << dirs_fant[k] << " --- " << x[k] <<endl;
      }
    }
    //for (int i = 0; i<8; ++i) cerr << x[i] << " ";
    //cerr <<endl;
    int min = 1000;
    int posmin = 0;
    for (int i = 0; i<8; ++i) {
      if (x[i]<min)  {
        min = x[i];
        posmin = i;
        }
    }
    //cerr << "distancia " << x[min];
    return dirs_mag[posmin];
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
        //cerr << "inicio_turno" << endl;
        vector<int> ws = wizards(me());
        for (int id : ws) {      
          Dir d = pos(id);
          //cerr << id << ": "<< unit(id).pos << " va a " << d << endl;
          move(id,d);
        } 
        int g = ghost(me());
        int rondes_per_atac = unit(g).resting_rounds();
        if (rondes_per_atac == 0) spell(g,arcane());
        else {
          //cerr << "fantasma" << endl;
          move (g,pos_f(g));
        }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
