#ifndef COMPONENTE_H
#define COMPONENTE_H

class Componente {
  public:
    virtual void iniciar() = 0;               // Configurar el componente
    virtual void escribir(int valor) = 0;     // Enviar señal (LED/LCD)
    virtual int leer() = 0;                   // Leer estado (sensor/botón)
    virtual ~Componente() {}                  // Destructor virtual
};

#endif