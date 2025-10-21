#include <iostream>

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    MatrizBase(int f, int c) : _filas(f), _columnas(c) {}
    virtual ~MatrizBase() {}
    
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    
    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T **_datos;

public:
    MatrizDinamica(int f, int c);
    ~MatrizDinamica();
    MatrizDinamica(const MatrizDinamica<T>& otra);
    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra);
    
    void cargarValores() override;
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override;
    void imprimir() const override;
    MatrizDinamica<T> operator+(const MatrizDinamica<T>& otra) const;
    
    void setValor(int f, int c, T valor);
    T getValor(int f, int c) const;
};

template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N];

public:
    MatrizEstatica();
    
    void cargarValores() override;
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override;
    void imprimir() const override;
    MatrizEstatica<T, M, N> operator+(const MatrizEstatica<T, M, N>& otra) const;
    
    void setValor(int f, int c, T valor);
    T getValor(int f, int c) const;
};

template <typename T>
MatrizDinamica<T>::MatrizDinamica(int f, int c) : MatrizBase<T>(f, c)
{
    _datos = new T*[this->_filas];
    for (int i = 0; i < this->_filas; i++) {
        _datos[i] = new T[this->_columnas];
        for (int j = 0; j < this->_columnas; j++) {
            _datos[i][j] = 0;
        }
    }
}

template <typename T>
MatrizDinamica<T>::~MatrizDinamica()
{
    std::cout << "Liberando memoria de Matriz Dinamica...\n";
    for (int i = 0; i < this->_filas; i++) {
        delete[] _datos[i];
    }
    delete[] _datos;
}

template <typename T>
MatrizDinamica<T>::MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(otra._filas, otra._columnas)
{
    _datos = new T*[this->_filas];
    for (int i = 0; i < this->_filas; i++) {
        _datos[i] = new T[this->_columnas];
        for (int j = 0; j < this->_columnas; j++) {
            _datos[i][j] = otra._datos[i][j];
        }
    }
}

template <typename T>
MatrizDinamica<T>& MatrizDinamica<T>::operator=(const MatrizDinamica<T>& otra)
{
    if (this != &otra) {
        for (int i = 0; i < this->_filas; i++) {
            delete[] _datos[i];
        }
        delete[] _datos;
        
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;
        
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; i++) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
    }
    return *this;
}

template <typename T>
void MatrizDinamica<T>::cargarValores()
{
    std::cout << "Ingrese valores para la matriz " << this->_filas << "x" << this->_columnas << ":\n";
    for (int i = 0; i < this->_filas; i++) {
        for (int j = 0; j < this->_columnas; j++) {
            std::cout << "[" << i << "][" << j << "]: ";
            std::cin >> _datos[i][j];
        }
    }
}

template <typename T>
MatrizBase<T>* MatrizDinamica<T>::sumar(const MatrizBase<T>& otra) const
{
    if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
        std::cout << "Error: Dimensiones incompatibles para suma.\n";
        return nullptr;
    }
    
    MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);
    
    const MatrizDinamica<T>* otraDin = dynamic_cast<const MatrizDinamica<T>*>(&otra);
    const MatrizEstatica<T, 3, 2>* otraEst32 = dynamic_cast<const MatrizEstatica<T, 3, 2>*>(&otra);
    const MatrizEstatica<T, 2, 2>* otraEst22 = dynamic_cast<const MatrizEstatica<T, 2, 2>*>(&otra);
    
    if (otraDin) {
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                resultado->_datos[i][j] = this->_datos[i][j] + otraDin->_datos[i][j];
            }
        }
    } else if (otraEst32) {
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                resultado->_datos[i][j] = this->_datos[i][j] + otraEst32->getValor(i, j);
            }
        }
    } else if (otraEst22) {
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                resultado->_datos[i][j] = this->_datos[i][j] + otraEst22->getValor(i, j);
            }
        }
    }
    
    return resultado;
}

template <typename T>
void MatrizDinamica<T>::imprimir() const
{
    for (int i = 0; i < this->_filas; i++) {
        std::cout << "| ";
        for (int j = 0; j < this->_columnas; j++) {
            std::cout << _datos[i][j] << " | ";
        }
        std::cout << "\n";
    }
}

template <typename T>
MatrizDinamica<T> MatrizDinamica<T>::operator+(const MatrizDinamica<T>& otra) const
{
    MatrizDinamica<T> resultado(this->_filas, this->_columnas);
    
    for (int i = 0; i < this->_filas; i++) {
        for (int j = 0; j < this->_columnas; j++) {
            resultado._datos[i][j] = this->_datos[i][j] + otra._datos[i][j];
        }
    }
    
    return resultado;
}

template <typename T>
void MatrizDinamica<T>::setValor(int f, int c, T valor)
{
    if (f >= 0 && f < this->_filas && c >= 0 && c < this->_columnas) {
        _datos[f][c] = valor;
    }
}

template <typename T>
T MatrizDinamica<T>::getValor(int f, int c) const
{
    if (f >= 0 && f < this->_filas && c >= 0 && c < this->_columnas) {
        return _datos[f][c];
    }
    return 0;
}

template <typename T, int M, int N>
MatrizEstatica<T, M, N>::MatrizEstatica() : MatrizBase<T>(M, N)
{
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            _datos[i][j] = 0;
        }
    }
}

template <typename T, int M, int N>
void MatrizEstatica<T, M, N>::cargarValores()
{
    std::cout << "Ingrese valores para la matriz " << M << "x" << N << ":\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << "[" << i << "][" << j << "]: ";
            std::cin >> _datos[i][j];
        }
    }
}

template <typename T, int M, int N>
MatrizBase<T>* MatrizEstatica<T, M, N>::sumar(const MatrizBase<T>& otra) const
{
    if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
        std::cout << "Error: Dimensiones incompatibles para suma.\n";
        return nullptr;
    }
    
    MatrizEstatica<T, M, N>* resultado = new MatrizEstatica<T, M, N>();
    
    const MatrizEstatica<T, M, N>* otraEst = dynamic_cast<const MatrizEstatica<T, M, N>*>(&otra);
    
    if (otraEst) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                resultado->_datos[i][j] = this->_datos[i][j] + otraEst->_datos[i][j];
            }
        }
    }
    
    return resultado;
}

template <typename T, int M, int N>
void MatrizEstatica<T, M, N>::imprimir() const
{
    for (int i = 0; i < M; i++) {
        std::cout << "| ";
        for (int j = 0; j < N; j++) {
            std::cout << _datos[i][j] << " | ";
        }
        std::cout << "\n";
    }
}

template <typename T, int M, int N>
MatrizEstatica<T, M, N> MatrizEstatica<T, M, N>::operator+(const MatrizEstatica<T, M, N>& otra) const
{
    MatrizEstatica<T, M, N> resultado;
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            resultado._datos[i][j] = this->_datos[i][j] + otra._datos[i][j];
        }
    }
    
    return resultado;
}

template <typename T, int M, int N>
void MatrizEstatica<T, M, N>::setValor(int f, int c, T valor)
{
    if (f >= 0 && f < M && c >= 0 && c < N) {
        _datos[f][c] = valor;
    }
}

template <typename T, int M, int N>
T MatrizEstatica<T, M, N>::getValor(int f, int c) const
{
    if (f >= 0 && f < M && c >= 0 && c < N) {
        return _datos[f][c];
    }
    return 0;
}

int main()
{
    std::cout << "=== Sistema Generico de Algebra Lineal ===\n\n";
    
    std::cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<\n\n";
    
    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    std::cout << "Creando Matriz Dinamica A (3x2)...\n";
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(0, 0, 1.5f);
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(0, 1, 2.0f);
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(1, 0, 0.0f);
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(1, 1, 1.0f);
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(2, 0, 4.5f);
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(2, 1, 3.0f);
    
    std::cout << "A =\n";
    A->imprimir();
    
    MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();
    std::cout << "\nCreando Matriz Estatica B (3x2)...\n";
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(0, 0, 0.5f);
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(0, 1, 1.0f);
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(1, 0, 2.0f);
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(1, 1, 3.0f);
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(2, 0, 1.0f);
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(2, 1, 1.0f);
    
    std::cout << "B =\n";
    B->imprimir();
    
    std::cout << "\nSUMANDO: Matriz C = A + B ...\n";
    std::cout << "(La suma es manejada por el metodo virtual de MatrizDinamica)\n\n";
    
    MatrizBase<float>* C = A->sumar(*B);
    
    std::cout << "Matriz Resultado C (3x2, Tipo FLOAT):\n";
    C->imprimir();
    
    std::cout << "\n>> Demostracion de Limpieza de Memoria <<\n";
    std::cout << "Llamando al destructor de C...\n";
    delete C;
    std::cout << "Llamando al destructor de A...\n";
    delete A;
    std::cout << "Llamando al destructor de B...\n";
    delete B;
    
    std::cout << "\n>> Demostracion de Genericidad (Tipo INT) <<\n\n";
    
    MatrizBase<int>* D = new MatrizDinamica<int>(2, 2);
    std::cout << "Creando Matriz Dinamica D (2x2)...\n";
    dynamic_cast<MatrizDinamica<int>*>(D)->setValor(0, 0, 5);
    dynamic_cast<MatrizDinamica<int>*>(D)->setValor(0, 1, 10);
    dynamic_cast<MatrizDinamica<int>*>(D)->setValor(1, 0, 15);
    dynamic_cast<MatrizDinamica<int>*>(D)->setValor(1, 1, 20);
    
    std::cout << "D =\n";
    D->imprimir();
    
    MatrizBase<int>* E = new MatrizDinamica<int>(2, 2);
    std::cout << "\nCreando Matriz Dinamica E (2x2)...\n";
    dynamic_cast<MatrizDinamica<int>*>(E)->setValor(0, 0, 1);
    dynamic_cast<MatrizDinamica<int>*>(E)->setValor(0, 1, 2);
    dynamic_cast<MatrizDinamica<int>*>(E)->setValor(1, 0, 3);
    dynamic_cast<MatrizDinamica<int>*>(E)->setValor(1, 1, 4);
    
    std::cout << "E =\n";
    E->imprimir();
    
    std::cout << "\nSUMANDO: Matriz F = D + E (Polimorfismo con INT)...\n\n";
    
    MatrizBase<int>* F = D->sumar(*E);
    
    std::cout << "Matriz Resultado F (2x2, Tipo INT):\n";
    F->imprimir();
    
    std::cout << "\n>> Demostracion de Sobrecarga del Operador + <<\n";
    std::cout << "Usando sintaxis natural: G = D + E\n\n";
    
    MatrizDinamica<int> G = *dynamic_cast<MatrizDinamica<int>*>(D) + *dynamic_cast<MatrizDinamica<int>*>(E);
    
    std::cout << "Matriz G (resultado usando operator+):\n";
    G.imprimir();
    
    std::cout << "\n>> Limpieza de Memoria <<\n";
    delete F;
    delete D;
    delete E;
    
    std::cout << "\nSistema cerrado.\n";
    return 0;
}