#include "Player.hh"
#include <list>
#include <vector>

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
  typedef vector<int> VI;
  typedef vector<VI> MI;
  VI zzzzz = VI(60,-1);
  MI visited = MI(60,zzzzz);
  map<int,Pos> lugar;

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

  Pos libro(int id) {
    queue<Pos> q;
    Pos p = unit(id).pos;
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
        //cerr << unit(id).pos.i << " " << unit(id).pos.j << " : " << p.i << " " <<p.j;
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
    cerr << "Diferencia: " << i << " " << j << " caso: ";
    if (i==j and i==0) {
      lugar.erase(id);
      cerr << "encontrado" << endl;
      return Up;
    }
    if (j>0 and pos_ok(p+Left) and cell(p+Left).type!=Wall) {
      cerr << 1 << endl;
      return Left;
    }
    if (j<0 and pos_ok(p+Right) and cell(p+Right).type!=Wall) {
    cerr << 2 << endl;
    return Right;
    }
    if (i<0 and pos_ok(p+Down) and cell(p+Down).type!=Wall) { 
    cerr << 3 << endl;
    return Down;
    }
    if (i>0 and pos_ok(p+Up) and cell(p+Up).type!=Wall) {
    cerr << 4 << endl;
    return Up;
    }
    if (i<0) {
    cerr << 5 << endl;
    return Up;
    }
    cerr << 6 << endl;
    return Down; 
    
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
        cerr << "inicio_turno" << endl;
        vector<int> ws = wizards(me());
        for (int id : ws) {
          map<int,Pos>::iterator it;
          it = lugar.find(id);
          // cerr << " encontrado" << endl;
          if (it==lugar.end()) {
            Pos p = libro(id);
            lugar.insert(pair<int,Pos>(id,p));
            Dir d = movimiento(id, unit(id).pos,p);
            cerr << id << ": "<< unit(id).pos <<" va a " << d << " para " << p << endl;
            move(id,d);
          }
          else {
            Pos p = it->second;
            Dir d = movimiento(id, unit(id).pos,p);
            cerr << id << ": "<< unit(id).pos <<" va a " << d << " para " << p << endl;
            move(id,d);
          }
          //move(id,mejor_mov_mago(id));
        }
        //for(map<int,Pos>::iterator it = lugar.begin(); it!=lugar.end(); ++it) {
        //  cerr << "id " << it->first << ": va a libro en " << it->second.i << " " << it->second.j << endl;
        //}
        int g = ghost(me());
        int rondes_per_atac = unit(g).resting_rounds();
        if (rondes_per_atac == 0) spell(g,arcane());
        

  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
