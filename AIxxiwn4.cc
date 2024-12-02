#include "Player.hh"
#include <list>
#include <vector>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME xxiwn4


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
  VI zzzzz = VI(60,-1);
  MI visited = MI(60,zzzzz);
  map<int,set<Pos>> lugar;
  map<int,bool> prio;

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

  bool bsearch(const vector<int>& A, const int& x,
    int l, int u) {
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



  Pos libro(int id) {
    queue<Pos> q;
    Pos x = unit(id).pos;
    Pos p = x;
    q.push(p);
    visited[p.i][p.j] = id;
    while (not q.empty()) {
      //cerr << endl;
      p = q.front();
      //cerr << p.i << " " << p.j << endl;
      //cerr << "---------------" << endl;
      q.pop();
      int i = p.i;
      int j = p.j;
      if (cell(i,j).book) { 
        //cerr << unit(id).pos.i << " " << unit(id).pos.j << " : " << p.i << " " <<p.j << endl;
        return p;
      }
      else {
        for (int k=0; k<4; ++k) {
          Pos pp = p+dirs_mag[k];
          if (pos_ok(pp) and cell(pp).type!=Wall and visited[pp.i][pp.j]!=id) {
            //cerr << pp.i << " " << pp.j;
            //cerr << " ok" << endl;
            q.push(pp);
            visited[pp.i][pp.j] = id;
          }
          //else if (cell(pp).type==Wall) cerr << pp.i << " " << pp.j << " X" << endl;
        }
      }
    }
    return unit(id).pos;
  }

  Dir movimiento(int id, Pos p, Pos dest) {
    int i = p.i - dest.i;
    int j = p.j - dest.j;
    cerr << "Diferencia: " << i << " " << j << endl;
    map<int,set<Pos>>::iterator l;
    map<int,bool>::iterator m;
    l = lugar.find(id);
    //No esta
    if (l==lugar.end()) {
      lugar.insert(pair<int,set<Pos>>(id,set<Pos>()));
      l = lugar.find(id);
      prio.insert(pair<int,bool>(id,false));
    }
    if (i==j and i==0) {
      lugar.erase(id);
      prio.erase(id);
      cerr << "encontrado" << endl;
      dest = libro(id);
      lugar.insert(pair<int,set<Pos>>(id,set<Pos>()));
      l = lugar.find(id);
      prio.insert(pair<int,bool>(id,false));
    }
    m = prio.find(id);
    set<Pos>::iterator it;
    l->second.insert(p);
    cerr << "pasa" << endl;
    if (j==0) m->second = true;
    if (not m->second) {
      if (j<0) {
        it = l->second.find(p+Right);
        cerr << "pasa" << endl;
        if (pos_ok(p+Right) and cell(p+Right).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
          return Right;
        if (i>0) {
          it = l->second.find(p+Up);
          if (pos_ok(p+Up) and cell(p+(Up)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
            return Up;
          it = l->second.find(p+Down);
          if (pos_ok(p+Down) and cell(p+(Down)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
            return Down;
        }
        if (i<0) {
          it = l->second.find(p+Down);
          if (pos_ok(p+Down) and cell(p+(Down)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
            return Down;
          it = l->second.find(p+Up);
          if (pos_ok(p+Up) and cell(p+(Up)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
            return Up;
        }
        return Left;
      }
      else {
        it = l->second.find(p+Left);
        cerr << "pasa" << endl;
        if (pos_ok(p+Left) and cell(p+Left).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
          return Left;
        if (i>0) {
          it = l->second.find(p+Up);
          if (pos_ok(p+Up) and cell(p+(Up)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
            return Up;
          it = l->second.find(p+Down);
          if (pos_ok(p+Down) and cell(p+(Down)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
            return Down;
        }
        if (i<0) {
          it = l->second.find(p+Down);
          if (pos_ok(p+Down) and cell(p+(Down)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
            return Down;
          it = l->second.find(p+Up);
          if (pos_ok(p+Up) and cell(p+(Up)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
            return Up;
        }
        return Right;
      }
    }
  else {
    if (i>0) {
      it = l->second.find(p+Up);
      if (pos_ok(p+Up) and cell(p+(Up)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1)  
        return Up;
      it = l->second.find(p+Left);
        if (pos_ok(p+Left) and cell(p+Left).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
          return Left;
      it = l->second.find(p+Right);
      if (pos_ok(p+Right) and cell(p+(Right)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
          return Right;
      return Down;
      }
    else {
      it = l->second.find(p+Down);
      if (pos_ok(p+Down) and cell(p+(Down)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
        return Down;
      if (j>0) {  
        it = l->second.find(p+Left);
          if (pos_ok(p+Left) and cell(p+Left).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
            return Left;
      }
      else {
        it = l->second.find(p+Right);
        if (pos_ok(p+Right) and cell(p+(Right)).type!=Wall and it==l->second.end() and cell(p+Right).id==-1) 
            return Right;
      }
      return Down;
    }
    }
    return Up;
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
        cerr << "inicio_turno" << endl;
        vector<int> ws = wizards(me());
        for (int id : ws) {      
          Pos p = libro(id);
          Dir d = movimiento(id,unit(id).pos,p);
          cerr << id << ": "<< unit(id).pos <<" va a " << p << endl;
          move(id,d);
        } 
        int g = ghost(me());
        int rondes_per_atac = unit(g).resting_rounds();
        if (rondes_per_atac == 0) spell(g,arcane());
        

  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
