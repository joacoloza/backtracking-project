#include<iostream>
#include<vector>
#include<cstdlib>
#include<map>
#include<set>
#include<algorithm>

int buscarMejorAsignacion(std::vector<std::vector<int>> &puntajes, std::map<int,int> &asignacionFinal,
                          std::map<int,bool> &arbitros, std::vector<std::pair<int,int>> &enfrentamientos,
                          int partido, int &metrica, bool usarPoda){

    int maxParcial = 0;
    int maxRestantePosible = (enfrentamientos.size() - partido - 1) * 20; //maximo que se podria acumular en todos los partidos que quedan
    int puntos = 0;

    if(partido == enfrentamientos.size())
        return 0;


    std::map<int,int> asignaciones = asignacionFinal;
    /*
     se hace una copia ya que se puede dar el caso donde encontremos el mejor puntaje pero
     de todas formas tenemos que seguir buscando ya que al buscar un maximo no sabemos nunca si llegamos
     ( a excepcion del raro caso donde se da el puntaje perfecto )
     entonces al pasarse por referencia el parametro (para poder accederlo desde fuera)
     al continuar la busqueda los llamados recursivos van a cambiar las asignaciones
     de esta forma despues chequeamos, si y solo si el puntaje es un nuevo maximo entonces se modifica el mapeo de asignaciones real
    */
    auto arbitro = arbitros.begin();

    while(arbitro != arbitros.end()){
        //posible mejora: este while hacerlo solo sobre los que estan disponibles
        //en lugar de sobre todos y luego verificar si esta disponible
        if(arbitro->second){ // arbitros => map<int NroArbitro,bool Disponible>, el second es si está disponible
            metrica++;
            arbitro->second = false;
            puntos = puntajes[enfrentamientos[partido].first][arbitro->first] + puntajes[enfrentamientos[partido].second][arbitro->first];
            if(maxRestantePosible + puntos > maxParcial || !usarPoda)
                puntos += buscarMejorAsignacion(puntajes,asignaciones,arbitros,enfrentamientos,partido+1,metrica, usarPoda);

            arbitro->second = true;
            if(puntos >= maxParcial){
                maxParcial = puntos;
                asignacionFinal = asignaciones;
                asignacionFinal[partido] = arbitro->first;
            }
        }
        arbitro++;
    }

    return maxParcial;
}

void enfrentamientosManuales(std::vector<std::pair<int,int>> & partidos, int cantEquipos){
    int carga;
    int carga2;
    std::cout<<"================================================================================"<<std::endl; //no verifica que no se ingresen repetidos
    std::cout<<"  - No se controla que no se repitan o el rango. Cuidado! Gracias! -"<<std::endl;
    std::cout<<"  - Los indices empiezan en 1 -"<<std::endl;
    for(int x = 0; x < cantEquipos/2; x++){
        std::cout<<"  Partido "<<x+1<<", primer equipo: ";
        std::cin>>carga;
        std::cout<<"  Partido "<<x+1<<", segundo equipo: ";
        std::cin>>carga2;
        std::pair<int,int> nuevo(carga-1,carga2-1);
        partidos.push_back(nuevo);
    }

}

void enfrentamientosPreCargados(std::vector<std::pair<int,int>> & partidos, int cantEquipos){
    for(int x = 0; x < cantEquipos/2; x++){
        partidos.push_back(std::pair<int,int>(x*2,(x*2)+1));
    }
}

void printAsignaciones(std::map<int,int> & asignaciones, std::vector<std::pair<int,int>> enfrentamientos){
    for(auto it = asignaciones.begin(); it != asignaciones.end(); it++){
        std::cout<<"Partido : ["<<it->first+1<<"] Arbitro: ["<<it->second+1<<"] | Equipos "<<enfrentamientos[it->first].first+1<<" Vs. "<<enfrentamientos[it->first].second+1<<std::endl;
    }
}

void printPuntajes(std::vector<std::vector<int>> & puntajes){
    int i = 0;
    std::cout<<"Arbitro  :  ";

    auto arbs = puntajes.begin();
    int size = arbs->size();

    for(int i = 0; i < size; i++){
        if(i < 10){
            std::cout<<" "<<i+1<<" ";
        }
        else{
            std::cout<<i+1<<" ";
        }
    }

    std::cout<<""<<std::endl;

    for(auto equipos = puntajes.begin(); equipos != puntajes.end(); equipos++){
        std::cout<<"Equipo "<<i+1<<" :  ";
        for(auto arbitros = equipos->begin(); arbitros != equipos->end(); arbitros++){
            if(*arbitros < 10){
                std::cout<<" "<<*arbitros<<" ";
            }
            else{
               std::cout<<*arbitros<<" ";
            }
        }
        i++;
        std::cout<<" "<<std::endl;
    }
}

void puntajesRandom(std::vector<std::vector<int>> & puntajes, int cantEquipos, int cantArbitros){
    std::vector<int> interno;
    int random;
    srand(-1);
    for(int i = 0; i < cantEquipos; i++){
        for(int j = 0; j < cantArbitros; j++){

            random = rand() % 11;
            interno.push_back(random);
        }
        puntajes.push_back(interno);
        interno.clear();
    }
}

void ejercicio(){
    std::vector<std::vector<int>> puntajes;
    std::map<int,bool> disponibles;
    std::map<int,int> asignaciones;
    std::vector<std::pair<int,int>> enfrentamientos;

    int iteracionesPoda = 0;
    int iteracionesSinPoda = 0;
    int cantEquipos = 0;
    int cantArbitros = 0;
    int seleccion=0;
    bool input = true;


    while(input){
        std::cout<<"================================================================================"<<std::endl;
        std::cout<<"  Cantidad de equipos para la fecha ? :";
        std::cin>> cantEquipos;
        std::cout<<"  Cantidad de arbitros para la fecha ? : ";
        std::cin>> cantArbitros;
        input = (!(cantEquipos % 2 == 0) || !(cantArbitros >= cantEquipos / 2));
    }

    std::cout<<"  Desea seleccionar manualmente el enfrentamiento entre los equipos ?"<<std::endl;
    std::cout<<"   (1=si / 2=no) :   ";
    std::cin>>seleccion;
    if(seleccion==1)
        enfrentamientosManuales(enfrentamientos,cantEquipos);
    else
        enfrentamientosPreCargados(enfrentamientos,cantEquipos);

    for(int i = 0; i < cantArbitros; i++)
        disponibles[i] = true;

    puntajesRandom(puntajes,cantEquipos,cantArbitros);
    std::cout<<""<<std::endl;

    std::cout<<"======================  Puntajes Asignados  ===================================="<<std::endl;
    printPuntajes(puntajes);
    std::cout<<""<<std::endl;

    std::cout<<"======================  Resultados  ============================================"<<std::endl;
    std::cout<<""<<std::endl;
    std::cout<<"   = Con Poda ="<<std::endl;
    int pjeSinPoda = buscarMejorAsignacion(puntajes,asignaciones,disponibles,enfrentamientos,0,iteracionesSinPoda,false);
    printAsignaciones(asignaciones,enfrentamientos);
    asignaciones.clear();
    std::cout<<""<<std::endl;

    std::cout<<"   = Sin Poda ="<<std::endl;
    int pjeConPoda = buscarMejorAsignacion(puntajes,asignaciones,disponibles,enfrentamientos,0,iteracionesPoda,true);
    printAsignaciones(asignaciones,enfrentamientos);
    asignaciones.clear();
    std::cout<<""<<std::endl;

    std::cout<<"Puntaje maximo sin poda : "<<pjeSinPoda<<", Iteraciones : "<<iteracionesSinPoda<<std::endl;
    std::cout<<"Puntaje maximo con poda : "<<pjeConPoda<<", Iteraciones : "<<iteracionesPoda<<std::endl;
}

int main(){
    ejercicio();
    return 0;
}

