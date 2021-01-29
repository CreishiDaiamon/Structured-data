#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <windows.h>

struct champ
{
    //Nombre del campeón    
    std::string nombre;
    //Vida                  //Estructura para guardar la información de los personajes
    int hp[4];
    //Damage
    int dam[4];
};

struct skills
{
    //Pasiva
    double p_dam[4];
    int p_cd[4];
    //Q
    int q_dam[4];
    int q_cd[4];
    //W
    int w_dam[4];
    int w_cd[4];            //Estructura para guardar las estadisticas de las habilidades            
    //E                     //dam = daño
    int e_dam[4];           //cd = cooldown = turnos en los que la habilidad se recarga 
    int e_cd[4];            //shield = escudo = vida adicional agregada sobre la vida
    int e_heal[4];          
    int e_shield[4];
    //R
    double r_dam[3];
    double r_heal[3];
};

struct enemy
{                               //Estadisticas de los enemigos
    std::string enemy_name;     //Nombre del enemigo
    int hp;                     //Vida del enemigo
    int dam;                    //Daño del enemigo
    int cd;                     //Turnos en los que el enemigo no ataca
    bool status_cc;             //Define si el enemigo esta en cc (si puede atacar o no, 0 = si puede                               
};                              // atacar y 1 = no puede atacar)


//Definiendo estructuras tipo champ (los personajes a elegir)
champ champions[3]
{
    {"Aatrox",{713,936,1370,2110},{68,80,104,145}},     //Vida, Daño
    {"Annie",{654,871,1297,2020},{54,61,73,95}},        //Vida, Daño
    {"Jhin",{723,956,1402,2166},{66,78,100,139}}        //Vida, Daño
};
    
//Definiendo estructuras tipo skills (habilidades de los personajes)
skills champskills[3]
{
    //Aatrox
    {{0.06,0.07,0.09,0.12},{7,6,4,3},                   //Pasiva: Daño, CD
    {70,90,110,130},{4,3,2,1},                          //Q: Daño,CD
    {30,40,50,70},{8,7,5,4},                            //W: Daño,CD
    {0,0,0,0},{5,4,3,2},{0,0,0,0},                      //E: Daño, CD, Escudo
    {0.20,0.30,0.40},{0.50,0.75,1.00}},                 //R: Daño, Curación
    //Annie
    {{0,0,0,0},{0,0,0,0},                               //Pasiva: Daño, CD
    {80,160,205,250},{1,1,1,1},                         //Q: Daño,CD     
    {70,115,205,250},{3,3,3,3},                         //W: Daño,CD
    {20,30,50,60},{7,6,5,4},{40,90,205,250},            //E: Daño, CD, Escudo
    {150,275,400},{0,0,0}},                             //R: Daño, Curación
    //Jhin
    {{15,20,20,25},{1,1,1,1},                           //Pasiva: Daño, CD
    {40,95,120,145},{6,5,4,3},                          //Q: Daño,CD
    {20,85,155,190},{7,7,7,7},                          //W: Daño,CD
    {20,80,200,260},{4,4,4,4},{0,0,0,0},                //E: Daño, CD, Escudo
    {50,125,200},{0,0,0}},                              //R: Daño, Curación
};
    //Definiendo estructuras tipo enemy (estadísticas de enemigos)
enemy enemies[6]{
    {"Minion",1036,44,1},                               //Nombre, Vida, Daño
    {"Gromp",1800,60,2},                                //Nombre, Vida, Daño
    {"Red Buff",2100,80,3},                             //Nombre, Vida, Daño
    {"Dragón",3500,100,3},                              //Nombre, Vida, Daño
    {"Heraldo",8000,120,4,},                            //Nombre, Vida, Daño
    {"Dragon Ancestral",6400,150,2},                    //Nombre, Vida, Daño
};

//Prototipos de Funciones
int Attack(champ&, skills&, enemy&, int);               //Función que selecciona entre otras 3 (Aatrox, Annie y Jhin)
                                                        //basado en el campeón elegido
int Selec();                                            //Función para seleccionar personajes
void s_print(const std::string&);                       //Permite imprimir caracteres uno a uno (Impresión continua)
void Rondas(champ&, skills&, enemy&, int);              //Se encarga de ejecutar las rondas (peleas de varios turnos contra un enemigo)
void FightWindow(champ&, skills&, enemy&, int);         //Una interfaz que muestra stats de vida y las habiidades disponibles

void Aatrox(champ&, skills&, enemy&, int);             
void Annie(champ&, skills&, enemy&, int);               //Funciones que ejecutan las habilidades de los personajes (3),
void Jhin(champ&, skills&, enemy&, int);                //bajan la vida de los enemigos dependiendo de la habilidad seleccionada
void cd_test(int &);                                    //Sirve para probar si una habilidad está en cooldown o no

void EnemyAttack(champ&, skills&, enemy&, int);         //Función que ejecuta el ataque del enemigo bajando la vida del jugador

//Variable Globales
int cooldowns[4]={0, 0, 0, 0};          //Array que contiene los cooldowns de la pasiva y 3 habilidades (q, w, e)

char habilidad;                         //Variable de entrada que define que habilidad se va a usar

bool r_used=0,e_status=0,e_used=0;      //Ayudan a determinar si ciertas habilidades están activas para otorgar sus efectos

int cd_check=0,                         //Ayuda en la función cd_test, comprobando que el valor cooldowns[cd_check] sea 0                       //
    ronda=1,                            //Determina el numero de ronda actual y elige los stats del enemigo del array enemies[ronda-1]
    num,                                //Num para elegir personaje
    champion,                           //Variable usada en la función Attack para determinar las habilidades a usar
    cont_p=0,
    cont_enemy=1,                       //Contadores: ayudan a determinar cuantos turnos han pasado. Se utilizan para verrificar
    cont_a_pas=1,                       //que los efectos de una habilidad siguen activos o que el número de turnos necesarios 
    cont_annie_e=2,                     //para que se active han sido completados
    cont_q=0,
    cont_aatrox_r=0;
//Función main
int main ()
{
    int lvl=0;                              //Indica el nivel del jugador
    std::cout<<"RPG DE LOL\n";              //Nombre del programa
    num= Selec()-1;                         //Ayuda a determinar las estadisticas del campeón seleccionado
    champion=num+1;                         //Ayuda a elegir el set de habilidades a utilizar (dependiendo del campeón elegido)
    champ playerc = champions[num];         //Se definen las estadisiticas base el campeón seleccionado
    skills players = champskills[num];      //Se definen las habilidades del campeón seleccionado
    
    std::system("cls");                     //Luego de seleccionar el personaje en el menu producido por
                                            // Selec(), se limpia la pantalla

    while(ronda<=6)                         //Se repite hasta que los 6 enemigos han sido derrotados
    {   
        for (int i = 0; i < 3; i++)
        {
            cooldowns[i]=0;                 //Al inicio de cada ronda, todas las habilidades vuelven a estar disponibles (CD=0)
        }

        enemy myenemy = enemies[ronda-1];           //Se definen las estadisticas del enemigo a enfrentar
        Rondas(playerc, players, myenemy, lvl);     //Se ejecuta la función Rondas
        
        if(playerc.hp<=0)
        {
            std::cout<<"Has perdido manco, vuelve a intentarlo.";   //Luego de finalizada una ronda, si el jugador ha perdido,
            return 0;                                               //se muestra un mensaje y se termina el programa.
        }
        ronda++;                //Se aumenta el número de ronda

        if(ronda==2 || ronda==4||ronda==6)                  //Determina si el jugador ha llegado a la ronda designada
        {                                                   //Para subir de nivel
            lvl++;
            s_print("\n¡Has subido de nivel!\n\nPresione enter para continuar");
            std::cin.ignore();
            std::cin.get();                                 //Un presiona enter para continuar, se encuentra en varias partes 
            std::system("cls");                             //del programa
        }

    }

    s_print("\n\nFelicidades, has completado el juego.\n\n¡Bienvenido a high elo!");
    s_print("\n\nGG EZ");                                                               //Mensaje de felicitaciones por
    s_print("\nGracias por jugar, manco.");                                             //completar el juego

    return 0;
}

//FIN FUNCIÓN MAIN

int Selec ()
{
    int champ;
    std::cout<<"Seleccione el campeon que desea"<<std::endl;        //Menú de selección
    std::cout<<"Aatrox[1]\tAnnie[2]\tJhin[3]"<<std::endl;
    std::cin>>champ;
    while (champ !=1 && champ !=2 && champ !=3)
    {
        std::cout<<"No seas maje, volve a intentar.";               //Validación de datos
        std::cin>>champ;
    }
    return champ;
}

void Rondas (champ &p, skills &s, enemy &e, int l)
{
    int turno=1;                            //Determina el turno actual dentro de la ronda     
    std::cout<<"El "<<e.enemy_name <<" ha aparecido!"<<std::endl;

    while (e.hp>0 && p.hp[l]>0) //Esta secuencia de instrucciones se repetirá hasta que uno muera
    {
        int cont_h_cd=0;                            //Cuenta cuantas habilidades están en CD (no disponibles)
        std::cout<<"Ronda "<<ronda<<std::endl;      //Encabezado 
        std::cout<<"Turno "<<turno<<std::endl;

        FightWindow(p, s, e, l);                    //Menú de combate

        for(int i=1;i<=3;i++)
        {
            if(cooldowns[i]!=0)                     //Verifica que hayan habilidades disponibles
            {                                       
                cont_h_cd++;                        //Al ver que una habilidad no está disponible aumenta el contador
            }
        }

        if(cont_h_cd==3)
        {                                                                   //Automáticamente asigna un valor cualquiera a
            std::cout<<"Todas las habilidades estan en cd"<<std::endl;      //habilidad para que el switch de las habilidades
            habilidad='c';                                                  //(no pregunta que habilidad quiere usar ya que no hay)
        }else
        {
            std::cout<<"Que habilidad desea usar?(Q,w,E,R)"<<std::endl;     //Menú de selección de habilidades
            std::cin>>habilidad;                                                
            while (habilidad != 'q' && habilidad != 'w' && habilidad != 'e' && habilidad != 'r' )
            {
                std::cout<<"La opcion seleccionada no esta dentro de las opciones. Seleccione 'q', 'w', 'e' o 'r'"  //Validación
                <<std::endl;                                                                                        //de datos
                std::cin>>habilidad;
            }
            
            cd_test (cd_check);             //Verifica que la habilidad seleccionada esté disponible

            while(cooldowns[cd_check]!=0)
            {
                if(cooldowns[cd_check]!=0)
                {
                    if(r_used==0)           //Verifica que el ultimate (habilidad r) este disponible
                    {
                        break;              //de ser sóí sigue el programa
                    
                        std::cout<<"La habilidad seleccionada está en cooldown. Seleccione otra: "<<std::endl;  //Volver a ingresar
                        std::cin>>habilidad;
                        while (habilidad != 'q' && habilidad != 'w' && habilidad != 'e' && habilidad != 'r' )
                        {
                            std::cout<<"La opcion seleccionada no esta dentro de las opciones. Seleccione 'q', 'w', 'e' o 'r'"
                            <<std::endl;                               
                            std::cin>>habilidad;                        //Validación de datos
                        }
                        std::cout<<cont_h_cd;
                    }
                cd_test(cd_check);          //Vuelve a ejecutar cd_test
                }
            
                //R nivel 3
                if(habilidad=='r'&& l==0 )      //Evita seleccionar r al nivel 1
                {
                    while(habilidad=='r')
                    {
                    std::cout<<"La habilidad seleccionada se desbloquea en un nivel superior. Seleccione otra: "<<std::endl;
                    std::cin>>habilidad;
                        while (habilidad != 'q' && habilidad != 'w' && habilidad != 'e' )
                        {
                        std::cout<<"La opcion seleccionada no esta dentro de las opciones. Seleccione 'q', 'w' o 'e' "
                        <<std::endl;
                        }
                    }
                }
            }

            std::system("cls");

            Attack(p, s, e, l);
            EnemyAttack(p, s, e, l);
            
            std::cout<<"\nHP enemigo: "<<e.hp<<"/"<<enemies[ronda-1].hp<<std::endl;     //Muestra los resultados del turno
            std::cout<<"\nHP player: "<<p.hp[l]<<"/"<<champions->hp[l]<<std::endl;
            turno++;

            s_print("\n\nPresione enter para continuar.");      //Presione enter para continuar
            std::cin.ignore();
            std::cin.get();
            std::system("cls");                
        }

        std::cout<<"FIN DE RONDA"<<std::endl;       //Mensaje que indica que terminó la RONDA
        ronda++;                                    //Aumenta el valor de ronda, pasa a la siguiente ronda

        s_print("\n\nPresione enter para continuar.");          //Presione enter para continuar
        std::cin.ignore();
        std::cin.get();
        std::system("cls");    
    }
}


void FightWindow(champ &p, skills &s, enemy &e, int l) 
{
    //Muestra la estadisticas del enemigo
    std::cout<<"Nombre del enemigo: "<<e.enemy_name<<std::endl;
    std::cout<<"Enemy HP: "<<e.hp<<"/"<<enemies[ronda-1].hp<<" Damage "<<e.dam<<std::endl;
    std::cout<<"HP player: "<<p.hp[l]<<"/"<<champions->hp[l]<<std::endl;
    
    //Muestra las estadisticas del personaje seleccionado
    std::cout<<"\nHabilidad\t"<<std::left<<std::setw(5)<<"Damage\t"<<std::left<<
    std::setw(5)<<"CD"<<std::left<<std::setw(5)<<"\tCC"<<std::setw(5)<<"\t"<<
    std::setw(5)<<"Shield"<<std::endl;                                                  
    std::cout<<"Pasiva\t\t"<<s.p_dam[l]<<"\t"<<cooldowns[0]<<"\t"<<"xd"<<std::endl;
    std::cout<<"Q\t\t"<<s.q_dam[l]<<"\t"<<cooldowns[1]<<"\t"<<"-"<<std::endl;
    std::cout<<"W\t\t"<<s.w_dam[l]<<"\t"<<cooldowns[2]<<"\t"<<"xd"<<"\t\t-"<<std::endl;
    std::cout<<"E\t\t"<<s.e_dam[l]<<"\t"<<cooldowns[3]<<"\t"<<"-"<<
    "\t\t"<<s.e_shield[l]<<std::endl;
    std::cout<<"R\t\t"<<s.r_dam[l]<<"\t"<<"xd"<<"\t"<<"-"<<"\t\t-"<<std::endl;
}

void cd_test(int &cd_check)
{
    switch(habilidad)                   
    {                                   
        case 'q':                       
            cd_check=1;
        break;
        case 'w':                       //Asigna un valor a la variable cd_check dependiendo de la habilidad seleccionada
            cd_check=2;                 //luego ocupa este valor para saber el enfriamiento de la habilidad en el array
        break;                          //cooldowns[] para comprobar que la habilidad seleccionada esta disponible
        case 'e':
            cd_check=3;
        break;
        case 'r':

        break;
    }
}

int Attack(champ &p, skills &s, enemy &e, int l){
    switch(champion)
    {
        case 1:
            Aatrox(p, s, e, l);  //Invoca a la función Aatrox en caso de que sea el personaje selecionado
            break;
        case 2:
            Annie(p, s, e, l);  //Invoca a la función Annie en caso de que sea el personaje selecionado
            break;
        case 3:
            Jhin(p, s, e, l);   //Invoca a la función Jhin en caso de que sea el personaje selecionado
            break;
    }
    return 0;
}

void EnemyAttack(champ &p, skills &s, enemy &e, int l)
{
    cont_enemy--;                               //Reduce el CD del ataque del enemigo
    std::cout<<"\n\n"<<cont_enemy<<"\n\n";
    if(e.status_cc==1)                          //Verifica si el enemigo está bajo efectos de control (si puede atacar o no)
    {
        std::cout<<"\n\nEnemigo stuneado. No puede moverse.";
        e.status_cc=0;
    }else if(cont_enemy<=0)                     //Se asegura que el enemigo que no esta bajo efectos de 
    {                                           //control ataque
    std::cout<<"\n\n"<<e.enemy_name<<" da un ataque basico."<<std::endl;
    p.hp[l]-=e.dam;
    cont_enemy=e.cd;
    }else                                       //Delay de ataque del enemigo (CD)
    {
    std::cout<<"El "<<e.enemy_name<< " esta cansado. No hace nada este turno."<<std::endl;
    }

    if(e_used==1)                                                   //Sentencia que provoca daño al enemigo cada que ataca
    {                                                               //Fuente:Habilidad 'e' de Annie
        std::cout<<"El ataque ha quemado a "<<e.enemy_name<<std::endl;
        e.hp-=s.e_dam[l];
        cont_annie_e--;                         //Reduce el número de turnos que este efecto está activo                
    }

    s_print("\n\nPresione enter para continuar.");
    std::cin.ignore();                                  //Presione enter para continuar
    std::cin.get();
    std::system("cls");
}

void Aatrox(champ &p, skills &s, enemy &e, int l)
{
    int TotalDamage;                    //Variable usada para el calculo de los daños porcentuales
    cont_a_pas--;                       //Contador de la disponibilidad de la pasiva

    if(cont_a_pas==0)                   //La pasiva está disponible y ejecuta este bloque
    {
        s_print("Aatrox usa 'Heraldo de la Destrucción'. Haciendo damage adicional y curándose.");std::cout<<std::endl;
        int damage=s.p_dam[l]*(enemies[ronda-1].hp);
        p.hp[l]+=damage;
        e.hp=e.hp-damage-p.dam[l];      //Conjunto de sentencias que hacen funcionar la pasiva de AAtrox
        s_print("Aatrox hace ");std::cout<<damage+p.dam[l];s_print(" de damage");std::cout<<std::endl;
        s_print("Aatrox se cura por ");std::cout<<damage<<std::endl;
        
        cont_a_pas=4;
    }else                               //Si no está disponible, solo hace un ataque básico
    {
        s_print("Aatrox da un ataque basico, haciendo ");std::cout<<p.dam[l];s_print(" de damage");std::cout<<std::endl;
        e.hp-=p.dam[l];
    }

    for(int i=0; i <= 3 ; i++)
    {
        if(cooldowns[i]!=0)
            cooldowns[i]--;             //Disminuye el CD de cada una de las habilidades no disponibles
    }

    switch(habilidad)
    {
        case 'q':                       //Sentencias para la activación de la 'q' de Aatrox
            cont_q++;
            switch(cont_q)
            {
                case 1:                 //Activación de la primera 'q' de Aatrox
                    if(e_status==0)
                    {
                        TotalDamage=1.5*s.q_dam[l];     //Hace daño extra si el estado de Aatrox es "a rango" (e_status=0)
                        s_print("Aatrox usa 'La Espada Darkin'. *CLINK* Has dado con el filo de la espada. Haces ");std::cout<<TotalDamage;s_print(" de damage");std::cout<<std::endl;
                        e.hp-=TotalDamage;
                    }else                               //Hace el daño normal de la habilidad
                    {
                        TotalDamage=s.q_dam[l];
                        s_print("Aatrox usa 'La Espada Darkin'. Haces ");std::cout<<TotalDamage;s_print(" de damage");std::cout<<std::endl;
                        e.hp-=TotalDamage;
                    }
                    cooldowns[1]=1;                 //Hay que esperar un turno para poder tirar la siguiente
                break;
                case 2:                 //Activación de la segunda 'q' de Aatrox
                    if(e_status==0)
                    {                                   //No hace daño si e_status=0 (está a rango)
                        s_print("Aatrox usa 'La Espada Darkin'. Pero fallo.");std::cout<<std::endl;
                    }else
                    {                                   //Hace daño extra si el estado de Aatrox es "a melee" (e_status=1)
                        TotalDamage=1.75*s.q_dam[l];
                        s_print("Aatrox usa 'La Espada Darkin'. *CLINK* Has dado con el filo de la espada. Haces ");std::cout<<TotalDamage;s_print(" de damage");std::cout<<std::endl;
                        e.hp-=TotalDamage;
                        cooldowns[1]=1;             //Hay que esperar un turno para poder tirar la siguiente
                    }
                break;
                case 3:                 //Activación de la tercera 'q' de Aatrox
                    if(e_status==0)
                    {                                   //No hace daño si e_status=0 (está a rango)
                        s_print("Aatrox usa 'La Espada Darkin'. Pero fallo.");
                    }else
                    {                                   //Hace daño extra si el estado de Aatrox es "a melee" (e_status=1)
                        TotalDamage=2*s.q_dam[l];       
                        s_print("Aatrox usa 'La Espada Darkin'. *CLINK* Has dado con el filo de la espada. Haces ");std::cout<<TotalDamage;s_print(" de damage");std::cout<<std::endl;
                        e.hp-=TotalDamage;
                        cont_q=0;
                        cooldowns[1]=2;             //CD de la habilidad al terminar
                    }
                break;
            }
        break;
        case 'w':                       //Sentencias para la activación de la 'w' de Aatrox
            TotalDamage=s.w_dam[l];
            s_print("Aatrox usa 'Cadenas Infernales'. Hace ");std::cout<<TotalDamage;s_print(" de damage.");std::cout<<std::endl;
            s_print("Aatrox ata el alma de su enemigo, lo cual lo estuneara dentro de 2 turnos.");std::cout<<std::endl;
            e.hp-=TotalDamage;
            cooldowns[2]=s.w_cd[l];
        break;
        case 'e':                       //Sentencias para la activación de la 'e' de Aatrox, la cual cambia e_status
            if(e_status==0)
            {
                s_print("Aatrox se desplaza hacia el enemigo usando 'Impulso Siniestro'.");std::cout<<std::endl;
                e_status=1;
            }else
            {
                s_print("Aatrox se aleja del enemigo usando 'Impulso Siniestro'.");std::cout<<std::endl;
                e_status=0;
            }
            cooldowns[3]=s.w_cd[l];
        break;
        case 'r':                       //Sentencias para la activación de la 'r' de Aatrox
            s_print("Aatrox usa 'Destructor de Mundos'. Aumentando su damage y curaciones.");std::cout<<std::endl;
            r_used=1;                   //Determina que la habilidad está activa
            cont_aatrox_r=8;            //Numero de turnos que permanece activo el efecto
        break;
        default:                        //En caso de no haber ninguna habilidad disponible
            s_print("Ninguna habilidad está disponible.");std::cout<<"\n";
        break;
    }

    if(r_used==1)                               //Verifica si la 'r' ha sido usada en esta ronda
    {                                           //en este caso hace daño extra y se cura un porcentaje del daño efectuado
        double TrueTotalDamage, ExtraDamage;
        ExtraDamage=TotalDamage*s.r_dam[l];     //Daño extra
        e.hp-=ExtraDamage;
        TrueTotalDamage=TotalDamage+ExtraDamage;
        s_print("Aatrox utiliza su poder demoniaco para hacer ");std::cout<<ExtraDamage,s_print(" de damage adicional\n");
        std::cout<<"Aatrox se cura por "<<s.r_heal[l]*TrueTotalDamage<<" de vida"<<std::endl;
        cont_aatrox_r--;
    }else if(cont_aatrox_r==0)                  //Informa al jugador que se han acabado los efectos de la 'r'
    {
        std::cout<<"\nLos efectos de 'Devorador de Mundos han acabado.";
        r_used=0;                               //Desactiva el efecto de la r
    }
}

void Annie(champ &p, skills &s, enemy &e, int l){
    int cont_e;         //Contador de los turnos que dura el efecto de la e de Annie

    for(int i=0; i <= 3 ; i++)
    {
        if(cooldowns[i]!=0)
            cooldowns[i]--;
    }        

    switch(habilidad)
    {
            //Sentencias de la 'q' de Annie
        case 'q':
            s_print("Has utilizado 'Desintegrar'. Haces ");std::cout<<s.q_dam[l];s_print(" de damage.");
            e.hp-=s.q_dam[l];
            cooldowns[1]=s.q_cd[l];
            cont_p++;
            break;
            //Sentencias de la 'w' de Annie
        case 'w':
            s_print("Has utilizado 'Incinerar'. Haces ");std::cout<<s.w_dam[l];s_print(" de damage.");
            e.hp=e.hp-s.w_dam[l];
            cooldowns[2]=s.w_cd[l];
            cont_p++;
            break;
            //Sentencias de la 'e' de Annie
        case 'e':
            s_print("Has utilizado 'Escudo Fundido'. Recibes un escudo de ");std::cout<<s.e_shield[l];s_print(" de vida.");std::cout<<"\n";
            s_print("Los enemigos reciben ");std::cout<<s.e_dam[l];s_print(" de damage al atacarte durante los siguientes 2 turnos.");std::cout<<"\n";
            p.hp[l] +=s.e_shield[l];
            e_used=1;
            cooldowns[3]=s.e_cd[l];
            cont_e=0;
            cont_p++;
            break;
            //Sentencias de la 'r' de Annie
        case 'r':
            s_print("Has utilizado 'Tibers'. Haces ");std::cout<< s.r_dam[l];s_print(" de damage. ");std::cout<<"\n";
            e.hp=e.hp-s.r_dam[l];
            cont_p++;
            r_used=1;
            break;
            //Sentencias en el caso de tener 0 habilidades disponibles
        default:
            s_print("Ninguna habilidad está disponible.");std::cout<<"\n";
        break;
    }

    if(cont_p==4)                                               //Sentencias que hacen funcionar la pasiva de Annie
    {                                                           //Al cuarto ataque el enemigo no podrá atacar
        s_print("\nHas estuneado al enemigo");std::cout<<"\n";
        e.status_cc=1;
        cont_p=0;
    }

    if(cont_e!=1)
    {
        cont_e++;
    }
    else                                                         //Determina los turnos que dura el escudo
    {
        s_print("\nEl escudo se ha desactivado");std::cout<<"\n";
        cont_e=0;
    }
    //AA    
    s_print("\nDas un ataque basico. Haces ");std::cout<<p.dam[l];s_print(" de damage.");std::cout<<"\n";
    e.hp-=p.dam[l];
}

void Jhin(champ &p, skills &s, enemy &e, int l)
{

    int totaldamage;
    double pextra[] = {0.15, 0.20, 0.25};
    //cd
    for(int i=0; i <= 3 ; i++)
    {
        if(cooldowns[i]!=0)
            cooldowns[i]--;
    }        

    switch(habilidad)
    {
            //Sentencias de la 'q' de Jhin
        case 'q':
            s_print("Has utilizado 'Granada Bailarina'. Haces ");std::cout<<s.q_dam[l];s_print(" de damage.");
            e.hp-=s.q_dam[l];
            cooldowns[1]=s.q_cd[l];
            break;
            //Sentencias de la 'w' de Jhin
        case 'w':
            s_print("Has utilizado 'Brote Mortal'. Haces ");std::cout<<s.w_dam[l];s_print(" de damage.");
            e.hp=e.hp-s.w_dam[l];
            cooldowns[2]=s.w_cd[l];
            break;
            //Sentencias de la 'e' de Jhin
        case 'e':
            s_print("Has utilizado 'Audiencia cautiva'. ");;std::cout<<"\n";
            s_print("Los enemigos reciben ");std::cout<<s.e_dam[l];s_print(" de damage ");std::cout<<"\n";
            p.hp[l] +=s.e_shield[l];
            cooldowns[3]=s.e_cd[l];

            break;
            //Sentencias de la 'r' de Jhin
        case 'r':
            s_print("Has utilizado 'LLamado a escena'. Haces  ");std::cout<< s.r_dam[l];s_print(" de damage. ");std::cout<<"\n";
            e.hp=e.hp-s.r_dam[l];
            s_print("Segundo disparo . Haces  ");std::cout<< s.r_dam[l];
            e.hp=e.hp-s.r_dam[l];
            s_print("Tecer disparo . Haces  ");std::cout<< s.r_dam[l];
            e.hp=e.hp-s.r_dam[l];
            totaldamage = s.r_dam[l]*2;
            s_print("1/4 disparo . Mi ingenio florece en la masacre como un lirio al amanecer. Haces  ")
            ;std::cout<< totaldamage;s_print(" de damage.");
            e.hp=e.hp-totaldamage;
            r_used=1;
            break;
            //Sentencias en el caso de tener 0 habilidades disponibles
        default:
            s_print("Ninguna habilidad esta disponible.");std::cout<<"\n";
        break;
    }
    totaldamage = p.dam[l];
    //Funcionamiento de la pasiva de Jhin, la cual en el 4to basico hace más daño 
        if(cont_p==4)
    {
        totaldamage += e.hp*pextra[l];
        s_print("\nCUATRO. Jhin da un basico potenciado ");std::cout<<"\n";
        s_print("\nDas un ataque basico. Haces ");std::cout<<totaldamage;s_print(" de damage.");std::cout<<"\n";
        e.hp-=totaldamage;
        cont_p=0;

    }
    else
    {
    //Ataques básicos de Jhin   
    s_print("\nDas un ataque basico. Haces ");std::cout<<totaldamage;s_print(" de damage.");std::cout<<"\n";
    e.hp-=totaldamage;
    cont_p++;
    }

}

void s_print(const std::string& str)
{
    for (size_t i = 0; i != str.size(); ++i)
    { 
        std::cout << str[i];                //función que hace que se impriman los caracteres
        Sleep(35);                          //1 a 1
    }
}
