/*Máquina de bebidas*/
#include <iostream>
#include <string>
#include <iomanip>
//estructura de bebidas
struct Data{
    std::string Noun;
    double cambio;
    int cantidad;
};
struct Drinks
{
    std::string Name;
    double cost;
    int avlblt;
};
//definición de la estructura machine
    Drinks machine [] = { 
                        {"(1)Coca-cola ",0.75, 20 },
                        {"(2)Root Beer", 0.75, 20},
                        {"(3)Lemon lime", 0.75, 20},
                        {"(4) Grape soda", 0.80, 20},
                        {"(5) Cream soda", 0.80, 20}
    };
//prototipos de función
int Seleccion();//seleccionar la bebida
Data Process(double, int);//Procesar información
void Final(Data&);//Funcion que netrega cambio
bool Valid (double x);//función para validar datos
void list ();//función para desplegar la lista de bebidas
int main (){
//declaración de variables
    char resp;
    Data c;
    bool validation;
    double dinero;
    int num;
//Proceso principal
    list();
            std::cout<<"Bienvenido a la maquina de bebidas. Desea una bebida? (Y/N)"<<std::endl;
    std::cin>>resp;
    while (resp=='Y' || resp =='y')
    {
    list ();
    num= Seleccion();
        std::cout<<"Ingrese la cantidad de dinero de la cual dispone"<<std::endl;
        std::cin>>dinero;
        Valid (dinero);
        validation = Valid(dinero);
        while (validation)
        {
            std::cout<<"La cantidad de dinero debe ser mayor a 0 y menor a 1"<<std::endl;
            std::cin>>dinero;
        }
        if (dinero < machine[num-1].cost )
        {
            std::cout<<"Usted no dispone de dinero suficiente para comprar la bebida"<<std::endl;
        }
        else if (machine[num-1].avlblt==0)
        {
            std::cout<<"Se ha agotado la bebida que usted desea"<<std::endl;
        }
        else{
        Process(dinero, num);
        c = Process(dinero, num);
        machine[num-1].avlblt=c.cantidad;
        Final(c);
        }
    std::cout<<"Bienvenido a la maquina de bebidas. Desea una bebida? (Y/N)"<<std::endl;
    std::cin>>resp;
    } 
    std::cout<<"Gracias por usar la maquina de bebidas, vuelva pronto"<<std::endl;
    return 0;
}
int Seleccion (){
    int count;
    std::cout<<"Ingrese el numero que corresponde a la bebida que desea"<<std::endl;
        std::cin>>count;
        return count;
}
Data Process(double x, int y){
    int num= y-1;
    Data Bebida = {machine[num].Name};
    Bebida.cambio= x-machine[num].cost;
    Bebida.cantidad=machine[num].avlblt-1;
    return Bebida;
}
void Final(Data &p){
    std::cout<<"Usted compro la bebida "<< p.Noun<<" recibira un cambio de $"<<p.cambio<<std::endl;
}
bool Valid (double x){
    if (x>1 || x <0){
    return true;
    }
    else
    {
    return false;
    }
}
void list (){
            std::cout<<"\t\t\tBienvenido a la maquina de bebidas"<<std::endl;
        std::cout<<"\t\t\tLista de bebidas disponibles"<<std::endl;
    //fila 1
        std::cout<<std::left<<std::setw(20)<<"Nombre de la Bebida"<<std::left<<
        std::setw(8)<<"Precio"<<std::left<<std::setw(8)<<"Disponibilidad"<<std::endl;  
    //fila 2
        std::cout<<std::left<<std::setw(20)<< machine[0].Name<<std::left<<
        std::setw(8)<<machine[0].cost<<std::left<<std::setw(8)<<machine[0].avlblt<<std::endl;
    //fila 3
        std::cout<<std::left<<std::setw(20)<< machine[1].Name<<std::left<<
        std::setw(8)<<machine[1].cost<<std::left<<std::setw(8)<<machine[1].avlblt<<std::endl;
    //fila 4
        std::cout<<std::left<<std::setw(20)<< machine[2].Name<<std::left<<
        std::setw(8)<<machine[2].cost<<std::left<<std::setw(8)<<machine[2].avlblt<<std::endl;
    //fila 5
        std::cout<<std::left<<std::setw(20)<< machine[3].Name<<std::left<<
        std::setw(8)<<machine[3].cost<<std::left<<std::setw(8)<<machine[3].avlblt<<std::endl;
    //fila 6
        std::cout<<std::left<<std::setw(20)<< machine[4].Name<<std::left<<
        std::setw(8)<<machine[4].cost<<std::left<<std::setw(8)<<machine[4].avlblt<<std::endl;
}