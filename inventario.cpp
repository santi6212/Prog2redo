#include <iostream>
#include <ctime> 
#include <cctype>
#include <cstring>
#include <iomanip>
#include <limits>
#include <locale.h>
#include <windows.h>

using namespace std;

//1.1 Estructura Producto
struct Producto {
    int id;                    // Identificador único (autoincremental)
    char codigo[20];           // Código del producto (ej: "PROD-001")
    char nombre[100];          // Nombre del producto
    char descripcion[200];     // Descripción del producto
    int idProveedor;           // ID del proveedor asociado
    float precio;              // Precio unitario
    int stock;                 // Cantidad en inventario
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
};

//1.2 Estructura Proveedor

struct Proveedor {
    int id;                    // Identificador único (autoincremental)
    char nombre[100];          // Nombre del proveedor
    char rif[20];              // RIF o identificación fiscal
    char telefono[20];         // Teléfono de contacto
    char email[100];           // Correo electrónico
    char direccion[200];       // Dirección física
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
};

//1.3 Estructura Cliente

struct Cliente {
    int id;                    // Identificador único (autoincremental)
    char nombre[100];          // Nombre completo del cliente
    char cedula[20];           // Cédula o RIF
    char telefono[20];         // Teléfono de contacto
    char email[100];           // Correo electrónico
    char direccion[200];       // Dirección física
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
};

//1.4 Estructura Transacción (CASO ESPECIAL: Esta estructura puede separarse como se comentó en clase, tienen libertad de hacerlo.)

struct Transaccion {
    int id;                    // Identificador único (autoincremental)
    char tipo[10];             // "COMPRA" o "VENTA"
    int idProducto;            // ID del producto involucrado
    int idRelacionado;         // ID del proveedor (compra) o cliente (venta)
    int cantidad;              // Cantidad de unidades
    float precioUnitario;      // Precio por unidad en esta transacción
    float total;               // cantidad * precioUnitario
    char fecha[11];            // Formato: YYYY-MM-DD
    char descripcion[200];     // Notas adicionales (opcional)
};

//1.5 Estructura Principal: Tienda

struct Tienda {
    char nombre[100];          // Nombre de la tienda
    char rif[20];              // RIF de la tienda
    
    // Arrays dinámicos de entidades
    Producto* productos;
    int numProductos;
    int capacidadProductos;
    
    Proveedor* proveedores;
    int numProveedores;
    int capacidadProveedores;
    
    Cliente* clientes;
    int numClientes;
    int capacidadClientes;
    
    Transaccion* transacciones;
    int numTransacciones;
    int capacidadTransacciones;
    
    // Contadores para IDs autoincrementales
    int siguienteIdProducto;
    int siguienteIdProveedor;
    int siguienteIdCliente;
    int siguienteIdTransaccion;
};

//==============
//verificaciones
//==============

string toLower(const char* s) {
    string r = s;
    for (char& c : r) c = tolower(c);
    return r;
}

bool contiene(const char* campo, const string& filtro) {
    string a = toLower(campo);
    return a.find(filtro) != string::npos;
}
bool existeProveedor(Tienda* tienda, int idProveedor) {
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (tienda->proveedores[i].id == idProveedor)
            return true;
    }
    return false;
}

bool codigoProductoDuplicado(Tienda* tienda, const char* codigo, int idIgnorar = -1) {
    for (int i = 0; i < tienda->numProductos; i++) {
        if (tienda->productos[i].id == idIgnorar) continue;
        if (strcmp(tienda->productos[i].codigo, codigo) == 0)
            return true;
    }
    return false;
}


void obtenerFechaActual(char* buffer) {
    time_t t = time(nullptr);
    tm* fecha = localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d",
            fecha->tm_year + 1900,
            fecha->tm_mon + 1,
            fecha->tm_mday);
}
const char* obtenerNombreProveedor(Tienda* tienda, int idProveedor) {
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (tienda->proveedores[i].id == idProveedor)
            return tienda->proveedores[i].nombre;
    }
    return "Desconocido";
}

void solicitarString(const char* mensaje, char* destino, int maxLen,
                     bool (*validador)(const char*) = nullptr,
                     const char* mensajeError = "ERROR: Valor inválido.\n") 
{
    while (true) {
        cout << mensaje;
        cin.getline(destino, maxLen);

        if (strlen(destino) == 0) {
            cout << "ERROR: No puede estar vacío.\n";
            continue;
        }

        if (validador != nullptr && !validador(destino)) {
            cout << mensajeError;
            continue;
        }

        break;
    }
}

bool sinEspacios(const char* texto) {
    for (int i = 0; texto[i] != '\0'; i++)
        if (texto[i] == ' ')
            return false;
    return true;
}


bool emailValido(const char* email) {
    const char* at = strchr(email, '@');
    if (!at) return false;

    const char* punto = strchr(at, '.');
    if (!punto) return false;

    return true;
}

bool rifDuplicado(Tienda* tienda, const char* rif, int idIgnorar = -1) {
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (tienda->proveedores[i].id == idIgnorar) continue;
        if (strcmp(tienda->proveedores[i].rif, rif) == 0)
            return true;
    }
    return false;
}

bool clienteDuplicado(Tienda* tienda, const char* cedula, int idIgnorar = -1) {
    for (int i = 0; i < tienda->numClientes; i++) {
        if (tienda->clientes[i].id == idIgnorar) continue;
        if (strcmp(tienda->clientes[i].cedula, cedula) == 0)
            return true;
    }
    return false;
}

bool confirmar(const char* mensaje) {
    char op[10];
    cout << mensaje;
    cin >> op;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // <- clave

    return toupper(op[0]) == 'S';
}


int solicitarEnteroPositivo(const char* mensaje) {
    int valor;
    cout << mensaje;
    cin >> valor;

    while (valor <= 0) {
        cout << "ERROR: Debe ser mayor a 0.\n";
        cout << mensaje;
        cin >> valor;
    }
    return valor;
}
int solicitarEnteroNoNegativo(const char* mensaje) {
    int valor;
    cout << mensaje;
    cin >> valor;

    while (valor < 0) {
        cout << "ERROR: Debe ser >= 0.\n";
        cout << mensaje;
        cin >> valor;
    }
    return valor;
}

int buscarProductoPorID(Tienda* tienda, int id) {
    for (int i = 0; i < tienda->numProductos; i++)
        if (tienda->productos[i].id == id)
            return i;
    return -1;
}

void mostrarProducto(const Producto& p) {
    cout << "Código: " << p.codigo << endl;
    cout << "Nombre: " << p.nombre << endl;
    cout << "Descripción: " << p.descripcion << endl;
    cout << "Proveedor ID: " << p.idProveedor << endl;
    cout << "Precio: " << p.precio << endl;
    cout << "Stock: " << p.stock << endl;
    cout << "Fecha: " << p.fechaRegistro << endl;
}

void redimensionarProductos(Tienda* tienda) {
    int nuevaCap = tienda->capacidadProductos * 2;
    Producto* nuevoArray = new Producto[nuevaCap];

    for (int i = 0; i < tienda->numProductos; i++) {
        nuevoArray[i] = tienda->productos[i];
    }

    delete[] tienda->productos;
    tienda->productos = nuevoArray;
    tienda->capacidadProductos = nuevaCap;
}

void redimensionarProveedores(Tienda* tienda) {
    int nuevaCap = tienda->capacidadProveedores * 2;
    Proveedor* nuevoArray = new Proveedor[nuevaCap];

    for (int i = 0; i < tienda->numProveedores; i++) {
        nuevoArray[i] = tienda->proveedores[i];
    }

    delete[] tienda->proveedores;
    tienda->proveedores = nuevoArray;
    tienda->capacidadProveedores = nuevaCap;
}

void redimensionarClientes(Tienda* tienda) {
    int nuevaCap = tienda->capacidadClientes * 2;
    Cliente* nuevoArray = new Cliente[nuevaCap];

    for (int i = 0; i < tienda->numClientes; i++) {
        nuevoArray[i] = tienda->clientes[i];
    }

    delete[] tienda->clientes;
    tienda->clientes = nuevoArray;
    tienda->capacidadClientes = nuevaCap;
}

void redimensionarTransacciones(Tienda* tienda) {
    int nuevaCap = tienda->capacidadTransacciones * 2;
    Transaccion* nuevoArray = new Transaccion[nuevaCap];

    for (int i = 0; i < tienda->numTransacciones; i++) {
        nuevoArray[i] = tienda->transacciones[i];
    }

    delete[] tienda->transacciones;
    tienda->transacciones = nuevoArray;
    tienda->capacidadTransacciones = nuevaCap;
}


//================
//tabla producto
//================

//linea horizontal
void linea(const char* tipo) {
    if (strcmp(tipo, "top") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
    else if (strcmp(tipo, "mid") == 0)
        cout << "¦--------------------------------------------------------------------------¦\n";
    else if (strcmp(tipo, "sep") == 0)
        cout << "¦----+-----------+------------------+--------------+-------+--------+------¦\n";
    else if (strcmp(tipo, "bot") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
}

//encabezado
void encabezadoProductos() {
    linea("top");
    cout << "¦                         LISTADO DE PRODUCTOS                             ¦\n";
    linea("mid");
    cout << "¦ ID ¦  Código   ¦     Nombre       ¦  Proveedor   ¦ Precio¦ Stock  ¦ Fecha¦\n";
    linea("sep");
}

//fila
void filaProducto(const Producto& p, const char* nombreProv) {
    cout << "¦ "
         << setw(2)  << left << p.id << " ¦ "
         << setw(9)  << left << p.codigo << " ¦ "
         << setw(16) << left << p.nombre << " ¦ "
         << setw(12) << left << nombreProv << " ¦ "
         << setw(5)  << left << p.precio << " ¦ "
         << setw(6)  << left << p.stock << " ¦ "
         << setw(4)  << left << p.fechaRegistro << " ¦\n";
}

void pieProductos() {
    linea("bot");
}

int buscarProveedorPorID(Tienda* tienda, int id) {
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (tienda->proveedores[i].id == id)
            return i;
    }
    return -1;
}

int buscarProveedorPorRIF(Tienda* tienda, const char* rif) {
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (strcmp(tienda->proveedores[i].rif, rif) == 0)
            return i;
    }
    return -1;
}

int buscarProveedorPorNombre(Tienda* tienda, const char* nombre) {
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (strstr(tienda->proveedores[i].nombre, nombre) != nullptr)
            return i;
    }
    return -1;
}

void mostrarProveedor(const Proveedor& p) {
    cout << "\n=== PROVEEDOR ENCONTRADO ===\n";
    cout << "ID: " << p.id << endl;
    cout << "RIF: " << p.rif << endl;
    cout << "Nombre: " << p.nombre << endl;
    cout << "Email: " << p.email << endl;
    cout << "Teléfono: " << p.telefono << endl;
}

//================
// tabla proveedor
//================
void lineaProv(const char* tipo) {
    if (strcmp(tipo, "top") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
    else if (strcmp(tipo, "mid") == 0)
        cout << "¦--------------------------------------------------------------------------¦\n";
    else if (strcmp(tipo, "sep") == 0)
        cout << "¦----+----------------------+------------------+---------------------------¦\n";
    else if (strcmp(tipo, "bot") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
}
void encabezadoProveedores() {
    lineaProv("top");
    cout << "¦                         LISTADO DE PROVEEDORES                           ¦\n";
    lineaProv("mid");
    cout << "¦ ID ¦       Nombre         ¦      RIF        ¦        Teléfono           ¦\n";
    lineaProv("sep");
}
void filaProveedor(const Proveedor& p) {
    cout << "¦ "
         << setw(2)  << left << p.id << " ¦ "
         << setw(20) << left << p.nombre << " ¦ "
         << setw(16) << left << p.rif << " ¦ "
         << setw(25) << left << p.telefono << " ¦\n";
}
void pieProveedores() {
    lineaProv("bot");
}

int buscarClientePorID(Tienda* tienda, int id) {
    for (int i = 0; i < tienda->numClientes; i++) {
        if (tienda->clientes[i].id == id)
            return i;
    }
    return -1;
}

int buscarClientePorCedula(Tienda* tienda, const char* cedula) {
    for (int i = 0; i < tienda->numClientes; i++) {
        if (strcmp(tienda->clientes[i].cedula, cedula) == 0)
            return i;
    }
    return -1;
}

int buscarClientePorNombre(Tienda* tienda, const char* nombre) {
    for (int i = 0; i < tienda->numClientes; i++) {
        if (strstr(tienda->clientes[i].nombre, nombre) != nullptr)
            return i;
    }
    return -1;
}

void mostrarCliente(const Cliente& c) {
    cout << "\n=== CLIENTE ENCONTRADO ===\n";
    cout << "ID: " << c.id << endl;
    cout << "Cédula/RIF: " << c.cedula << endl;
    cout << "Nombre: " << c.nombre << endl;
    cout << "Email: " << c.email << endl;
    cout << "Teléfono: " << c.telefono << endl;
    cout << "Dirección: " << c.direccion << endl;
}


//===============
//2.1 inicializar
//===============

void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif){

    strcpy(tienda->nombre, nombre);
    strcpy(tienda->rif, rif);
    
    tienda->capacidadProductos = 5;
    tienda->capacidadProveedores = 5;
    tienda->capacidadClientes = 5;
    tienda->capacidadTransacciones = 5;

    // Contadores
    tienda->numProductos = 0;
    tienda->numProveedores = 0;
    tienda->numClientes = 0;
    tienda->numTransacciones = 0;

    // IDs 
    tienda->siguienteIdProducto = 1;
    tienda->siguienteIdProveedor = 1;
    tienda->siguienteIdCliente = 1;
    tienda->siguienteIdTransaccion = 1;

    // Reservar memoria dinámica
    tienda->productos = new Producto[tienda->capacidadProductos];
    tienda->proveedores = new Proveedor[tienda->capacidadProveedores];
    tienda->clientes = new Cliente[tienda->capacidadClientes];
    tienda->transacciones = new Transaccion[tienda->capacidadTransacciones];

}

//delete
void liberarTienda(Tienda* tienda) {
    // Liberar memoria dinámica
    delete[] tienda->productos;
    delete[] tienda->proveedores;
    delete[] tienda->clientes;
    delete[] tienda->transacciones;

    // Dejar punteros en nullptr por seguridad
    tienda->productos = nullptr;
    tienda->proveedores = nullptr;
    tienda->clientes = nullptr;
    tienda->transacciones = nullptr;

    // Reiniciar contadores
    tienda->numProductos = 0;
    tienda->numProveedores = 0;
    tienda->numClientes = 0;
    tienda->numTransacciones = 0;
}

//======================
//2.2.1
//======================

void crearProducto(Tienda* tienda) {
    if (!confirmar("¿Desea registrar un nuevo producto? (S/N): "))
        return;

    Producto nuevo;

    // --- Código ---
   solicitarString(
    "Ingrese código del producto (o CANCELAR): ",
    nuevo.codigo,
    20,
    sinEspacios,
    "ERROR: El código no puede contener espacios.\n"
);

if (strcmp(nuevo.codigo, "CANCELAR") == 0 || strcmp(nuevo.codigo, "0") == 0)
    return;

if (codigoProductoDuplicado(tienda, nuevo.codigo)) {
    cout << "ERROR: El código '" << nuevo.codigo << "' ya está registrado.\n";
    return;
}

    // --- Nombre ---
    solicitarString("Ingrese nombre del producto (o CANCELAR): ", nuevo.nombre, 100);
    if (strcmp(nuevo.nombre, "CANCELAR") == 0 || strcmp(nuevo.nombre, "0") == 0)
        return;

    // --- Descripción ---
    solicitarString("Ingrese descripción (o CANCELAR): ", nuevo.descripcion, 200);
    if (strcmp(nuevo.descripcion, "CANCELAR") == 0 || strcmp(nuevo.descripcion, "0") == 0)
        return;

    // --- ID Proveedor ---
    cout << "Ingrese ID del proveedor (o 0 para cancelar): ";
    cin >> nuevo.idProveedor;

    if (nuevo.idProveedor == 0)
        return;

    if (!existeProveedor(tienda, nuevo.idProveedor)) {
        cout << "ERROR: El proveedor con ID " << nuevo.idProveedor << " no existe.\n";
        return;
    }

    // --- Precio ---
    nuevo.precio = solicitarEnteroPositivo("Ingrese precio (>0): ");

    // --- Stock ---
    nuevo.stock = solicitarEnteroNoNegativo("Ingrese stock (>=0): ");

    // Fecha automática
    obtenerFechaActual(nuevo.fechaRegistro);

    // --- Resumen ---
    cout << "\n=== RESUMEN DEL PRODUCTO ===\n";
    mostrarProducto(nuevo);

    if (!confirmar("¿Guardar producto? (S/N): "))
        return;

    // --- Redimensionar si es necesario ---
    if (tienda->numProductos >= tienda->capacidadProductos) {
    redimensionarProductos(tienda);
}


    // Guardado final
    nuevo.id = tienda->siguienteIdProducto++;
    tienda->productos[tienda->numProductos++] = nuevo;

    cout << "\nProducto registrado exitosamente.\n";
}

//========================
//2.2.2
//========================

void buscarProducto(Tienda* tienda) {
    int opcion;
    cout << "\n=== BUSCAR PRODUCTO ===\n";
    cout << "1. Buscar por ID\n";
    cout << "2. Buscar por nombre (parcial)\n";
    cout << "3. Buscar por código (parcial)\n";
    cout << "4. Listar por proveedor\n";
    cout << "0. Cancelar\n";
    cout << "Seleccione una opción: ";
    cin >> opcion;

    if (opcion == 0) return;

    char buffer[200];
    string filtro;

    switch (opcion) {

    // 1. Buscar por ID
    case 1: {
        int id;
        cout << "Ingrese ID del producto: ";
        cin >> id;

        int index = buscarProductoPorID(tienda, id);
        if (index == -1) {
            cout << "ERROR: No se encontró un producto con ID " << id << ".\n";
            return;
        }

        cout << "\n=== PRODUCTO ENCONTRADO ===\n";
        mostrarProducto(tienda->productos[index]);
        return;
    }

    // 2. Buscar por nombre (parcial)
    case 2: {
        solicitarString("Ingrese parte del nombre: ", buffer, 200);
        filtro = toLower(buffer);

        bool encontrado = false;
        cout << "\n=== RESULTADOS ===\n";

        for (int i = 0; i < tienda->numProductos; i++) {
            if (contiene(tienda->productos[i].nombre, filtro)) {
                mostrarProducto(tienda->productos[i]);
                cout << "-----------------------------\n";
                encontrado = true;
            }
        }

        if (!encontrado)
            cout << "No se encontraron coincidencias.\n";

        return;
    }

    // 3. Buscar por código (parcial)
    case 3: {
        solicitarString("Ingrese parte del código: ", buffer, 200);
        filtro = toLower(buffer);

        bool encontrado = false;
        cout << "\n=== RESULTADOS ===\n";

        for (int i = 0; i < tienda->numProductos; i++) {
            if (contiene(tienda->productos[i].codigo, filtro)) {
                mostrarProducto(tienda->productos[i]);
                cout << "-----------------------------\n";
                encontrado = true;
            }
        }

        if (!encontrado)
            cout << "No se encontraron coincidencias.\n";

        return;
    }

    // 4. Listar por proveedor
    case 4: {
        int idProv;
        cout << "Ingrese ID del proveedor: ";
        cin >> idProv;

        if (!existeProveedor(tienda, idProv)) {
            cout << "ERROR: El proveedor con ID " << idProv << " no existe.\n";
            return;
        }

        bool encontrado = false;
        cout << "\n=== PRODUCTOS DEL PROVEEDOR " 
             << obtenerNombreProveedor(tienda, idProv) << " ===\n";

        for (int i = 0; i < tienda->numProductos; i++) {
            if (tienda->productos[i].idProveedor == idProv) {
                mostrarProducto(tienda->productos[i]);
                cout << "-----------------------------\n";
                encontrado = true;
            }
        }

        if (!encontrado)
            cout << "No hay productos asociados a ese proveedor.\n";

        return;
    }

    default:
        cout << "Opción inválida.\n";
        return;
    }
}

//========================
//2.2.3
//========================

void actualizarProducto(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del producto a actualizar: ";
    cin >> id;

    int index = buscarProductoPorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un producto con ese ID.\n";
        return;
    }

    Producto* original = &tienda->productos[index];
    Producto temp = *original; // copia temporal

    int opcion;

    do {
        cout << "\n=== EDITAR PRODUCTO ===\n";
        cout << "1. Código: " << temp.codigo << endl;
        cout << "2. Nombre: " << temp.nombre << endl;
        cout << "3. Descripción: " << temp.descripcion << endl;
        cout << "4. Proveedor ID: " << temp.idProveedor << endl;
        cout << "5. Precio: " << temp.precio << endl;
        cout << "6. Stock: " << temp.stock << endl;
        cout << "7. Guardar cambios\n";
        cout << "0. Cancelar sin guardar\n";
        cout << "Opción: ";
        cin >> opcion;

        cin.ignore(); // limpiar buffer

        switch (opcion) {

        case 1: { // Código
            char nuevoCodigo[20];
            solicitarString("Nuevo código: ", nuevoCodigo, 20);

            if (codigoProductoDuplicado(tienda, nuevoCodigo, temp.id)) {
                cout << "ERROR: El código '" << nuevoCodigo << "' ya está registrado.\n";
            } else {
                strcpy(temp.codigo, nuevoCodigo);
            }
            break;
        }

        case 2: // Nombre
            solicitarString("Nuevo nombre: ", temp.nombre, 100);
            break;

        case 3: // Descripción
            solicitarString("Nueva descripción: ", temp.descripcion, 200);
            break;

        case 4: { // Proveedor
            int nuevoProv;
            cout << "Nuevo ID de proveedor: ";
            cin >> nuevoProv;

            if (!existeProveedor(tienda, nuevoProv)) {
                cout << "ERROR: El proveedor con ID " << nuevoProv << " no existe.\n";
            } else {
                temp.idProveedor = nuevoProv;
            }
            break;
        }

        case 5: // Precio
            temp.precio = solicitarEnteroPositivo("Nuevo precio (>0): ");
            break;

        case 6: // Stock
            temp.stock = solicitarEnteroNoNegativo("Nuevo stock (>=0): ");
            break;

        case 7: // Guardar cambios
            cout << "\n=== RESUMEN FINAL ===\n";
            mostrarProducto(temp);

            if (confirmar("¿Confirmar cambios? (S/N): ")) {
                *original = temp;
                cout << "Cambios guardados exitosamente.\n";
            } else {
                cout << "Cambios descartados.\n";
            }
            return;

        case 0:
            cout << "Cancelado. No se guardaron cambios.\n";
            return;

        default:
            cout << "Opción inválida.\n";
        }

    } while (true);
}

//========================
//2.2.4
//========================

void actualizarStockProducto(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del producto: ";
    cin >> id;

    int index = buscarProductoPorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un producto con ese ID.\n";
        return;
    }

    Producto* p = &tienda->productos[index];

    cout << "\n=== ACTUALIZAR STOCK ===\n";
    cout << "Producto: " << p->nombre << endl;
    cout << "Stock actual: " << p->stock << endl;

    int ajuste;
    cout << "Ingrese ajuste (+ para aumentar, - para disminuir): ";
    cin >> ajuste;

    int nuevoStock = p->stock + ajuste;

    if (nuevoStock < 0) {
        cout << "ERROR: Stock insuficiente. Disponible: " 
             << p->stock << ", Solicitado: " << (-ajuste) << endl;
        return;
    }

    cout << "Stock final sería: " << nuevoStock << endl;

    if (!confirmar("¿Confirmar cambio? (S/N): "))
        return;

    p->stock = nuevoStock;
    cout << "Stock actualizado exitosamente.\n";
}

//========================
//2.2.5
//========================

void listarProductos(Tienda* tienda) {
    if (tienda->numProductos == 0) {
        cout << "No hay productos registrados.\n";
        return;
    }

    encabezadoProductos();

    for (int i = 0; i < tienda->numProductos; i++) {
        Producto& p = tienda->productos[i];
        const char* nombreProv = obtenerNombreProveedor(tienda, p.idProveedor);
        filaProducto(p, nombreProv);
    }

    pieProductos();

    cout << "\nTotal de productos: " << tienda->numProductos << "\n";
}


//========================
//2.2.6
//========================

void eliminarProducto(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del producto a eliminar: ";
    cin >> id;

    int index = buscarProductoPorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un producto con ese ID.\n";
        return;
    }

    Producto& p = tienda->productos[index];

    cout << "\n=== PRODUCTO A ELIMINAR ===\n";
    mostrarProducto(p);

    // Advertencia por transacciones asociadas
    bool tieneTransacciones = false;
    for (int i = 0; i < tienda->numTransacciones; i++) {
        if (tienda->transacciones[i].idProducto == id) {
            tieneTransacciones = true;
            break;
        }
    }

    if (tieneTransacciones) {
        cout << "\nADVERTENCIA: Este producto tiene transacciones asociadas.\n";
    }

    if (!confirmar("\n¿Eliminar producto? (S/N): ")) {
        cout << "Eliminación cancelada.\n";
        return;
    }

    // Eliminar moviendo elementos
    for (int i = index; i < tienda->numProductos - 1; i++) {
        tienda->productos[i] = tienda->productos[i + 1];
    }

    tienda->numProductos--;

    cout << "Producto eliminado exitosamente.\n";
}

//2.3

void crearProveedor(Tienda* tienda) {
    if (!confirmar("¿Desea registrar un nuevo proveedor? (S/N): "))
        return;

    Proveedor nuevo;

    // --- RIF ---
    solicitarString(
        "Ingrese RIF del proveedor (o CANCELAR): ",
        nuevo.rif,
        20,
        nullptr
    );

    if (strcmp(nuevo.rif, "CANCELAR") == 0 || strcmp(nuevo.rif, "0") == 0)
        return;

    if (rifDuplicado(tienda, nuevo.rif)) {
        cout << "ERROR: El RIF '" << nuevo.rif << "' ya está registrado.\n";
        return;
    }

    // --- Nombre ---
    solicitarString("Ingrese nombre del proveedor (o CANCELAR): ",
                    nuevo.nombre, 100);

    if (strcmp(nuevo.nombre, "CANCELAR") == 0 || strcmp(nuevo.nombre, "0") == 0)
        return;

    // --- Email ---
    solicitarString(
        "Ingrese email del proveedor (o CANCELAR): ",
        nuevo.email,
        100,
        emailValido,
        "ERROR: El email debe contener '@'.\n"
    );

    if (strcmp(nuevo.email, "CANCELAR") == 0 || strcmp(nuevo.email, "0") == 0)
        return;

    // --- Teléfono ---
    solicitarString("Ingrese teléfono del proveedor (o CANCELAR): ",
                    nuevo.telefono, 20);

    if (strcmp(nuevo.telefono, "CANCELAR") == 0 || strcmp(nuevo.telefono, "0") == 0)
        return;

    // --- Confirmación ---
    cout << "\n=== RESUMEN DEL PROVEEDOR ===\n";
    cout << "RIF: " << nuevo.rif << endl;
    cout << "Nombre: " << nuevo.nombre << endl;
    cout << "Email: " << nuevo.email << endl;
    cout << "Teléfono: " << nuevo.telefono << endl;

    if (!confirmar("¿Guardar proveedor? (S/N): "))
        return;

    // --- Redimensionar si es necesario ---
    if (tienda->numProveedores >= tienda->capacidadProveedores)
        redimensionarProveedores(tienda);

    // --- Guardar ---
    nuevo.id = tienda->siguienteIdProveedor++;
    tienda->proveedores[tienda->numProveedores++] = nuevo;

    cout << "Proveedor registrado exitosamente.\n";
}

//2.3.2

void buscarProveedor(Tienda* tienda) {
    int opcion;

    do {
        cout << "\n=== BUSCAR PROVEEDOR ===\n";
        cout << "1. Buscar por ID\n";
        cout << "2. Buscar por nombre (coincidencia parcial)\n";
        cout << "3. Buscar por RIF\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcion == 0)
            return;

        int index = -1;

        switch (opcion) {

            case 1: {
                int id;
                cout << "Ingrese ID del proveedor: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                index = buscarProveedorPorID(tienda, id);
                break;
            }

            case 2: {
                char nombre[100];
                solicitarString("Ingrese nombre o parte del nombre: ", nombre, 100);
                index = buscarProveedorPorNombre(tienda, nombre);
                break;
            }

            case 3: {
                char rif[20];
                solicitarString("Ingrese RIF del proveedor: ", rif, 20);
                index = buscarProveedorPorRIF(tienda, rif);
                break;
            }

            default:
                cout << "Opción inválida.\n";
                continue;
        }

        if (index == -1) {
            cout << "No se encontró ningún proveedor con ese criterio.\n";
        } else {
            mostrarProveedor(tienda->proveedores[index]);
        }

        system("pause");

    } while (opcion != 0);
}

//2.3.3

void actualizarProveedor(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del proveedor a actualizar (0 para cancelar): ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (id == 0)
        return;

    int index = buscarProveedorPorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un proveedor con ese ID.\n";
        return;
    }

    Proveedor& original = tienda->proveedores[index];
    Proveedor temp = original; // copia temporal

    cout << "\n=== PROVEEDOR ENCONTRADO ===\n";
    mostrarProveedor(original);

    if (!confirmar("\n¿Desea modificar este proveedor? (S/N): "))
        return;

    // --- Nombre ---
    cout << "\nNombre actual: " << temp.nombre << endl;
    solicitarString("Nuevo nombre (ENTER para mantener): ", temp.nombre, 100);
    if (strlen(temp.nombre) == 0)
        strcpy(temp.nombre, original.nombre);

    // --- RIF ---
    cout << "\nRIF actual: " << temp.rif << endl;
    solicitarString("Nuevo RIF (ENTER para mantener): ", temp.rif, 20);
    if (strlen(temp.rif) == 0)
        strcpy(temp.rif, original.rif);
    else if (rifDuplicado(tienda, temp.rif, original.id)) {
        cout << "ERROR: El RIF '" << temp.rif << "' ya está registrado.\n";
        return;
    }

    // --- Teléfono ---
    cout << "\nTeléfono actual: " << temp.telefono << endl;
    solicitarString("Nuevo teléfono (ENTER para mantener): ", temp.telefono, 20);
    if (strlen(temp.telefono) == 0)
        strcpy(temp.telefono, original.telefono);

    // --- Email ---
    cout << "\nEmail actual: " << temp.email << endl;
    solicitarString("Nuevo email (ENTER para mantener): ", temp.email, 100);
    if (strlen(temp.email) == 0)
        strcpy(temp.email, original.email);
    else if (!emailValido(temp.email)) {
        cout << "ERROR: El email debe contener '@' y un dominio.\n";
        return;
    }

    // --- Dirección ---
    cout << "\nDirección actual: " << temp.direccion << endl;
    solicitarString("Nueva dirección (ENTER para mantener): ", temp.direccion, 200);
    if (strlen(temp.direccion) == 0)
        strcpy(temp.direccion, original.direccion);

    // --- Confirmación final ---
    cout << "\n=== NUEVOS DATOS DEL PROVEEDOR ===\n";
    mostrarProveedor(temp);

    if (!confirmar("¿Guardar cambios? (S/N): "))
        return;

    original = temp;
    cout << "Proveedor actualizado exitosamente.\n";
}

//2.3.4

void listarProveedores(Tienda* tienda) {
    if (tienda->numProveedores == 0) {
        cout << "No hay proveedores registrados.\n";
        return;
    }

    encabezadoProveedores();

    for (int i = 0; i < tienda->numProveedores; i++) {
        filaProveedor(tienda->proveedores[i]);
    }

    pieProveedores();

    cout << "\nTotal de proveedores: " << tienda->numProveedores << "\n";
}

//2.3.5

void eliminarProveedor(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del proveedor a eliminar (0 para cancelar): ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (id == 0)
        return;

    int index = buscarProveedorPorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un proveedor con ese ID.\n";
        return;
    }

    Proveedor& p = tienda->proveedores[index];

    cout << "\n=== PROVEEDOR A ELIMINAR ===\n";
    mostrarProveedor(p);

    // --- Verificar si tiene productos asociados ---
    bool tieneProductos = false;

    for (int i = 0; i < tienda->numProductos; i++) {
        if (tienda->productos[i].idProveedor == id) {
            tieneProductos = true;
            break;
        }
    }

    if (tieneProductos) {
        cout << "\nADVERTENCIA: Este proveedor tiene productos asociados.\n";
        cout << "No se puede eliminar mientras existan productos vinculados.\n";
        return;
    }

    // --- Confirmación ---
    if (!confirmar("\n¿Eliminar proveedor? (S/N): ")) {
        cout << "Eliminación cancelada.\n";
        return;
    }

    // --- Eliminar moviendo elementos ---
    for (int i = index; i < tienda->numProveedores - 1; i++) {
        tienda->proveedores[i] = tienda->proveedores[i + 1];
    }

    tienda->numProveedores--;

    cout << "Proveedor eliminado exitosamente.\n";
}

//=======================
//2.4
//=======================

void crearCliente(Tienda* tienda) {
    if (!confirmar("¿Desea registrar un nuevo cliente? (S/N): "))
        return;

    Cliente nuevo;

    // --- Cédula / RIF ---
    solicitarString(
        "Ingrese cédula/RIF del cliente (o CANCELAR): ",
        nuevo.cedula,
        20
    );

    if (strcmp(nuevo.cedula, "CANCELAR") == 0 || strcmp(nuevo.cedula, "0") == 0)
        return;

    if (clienteDuplicado(tienda, nuevo.cedula)) {
        cout << "ERROR: La cédula/RIF '" << nuevo.cedula << "' ya está registrada.\n";
        return;
    }

    // --- Nombre ---
    solicitarString("Ingrese nombre del cliente (o CANCELAR): ",
                    nuevo.nombre, 100);

    if (strcmp(nuevo.nombre, "CANCELAR") == 0 || strcmp(nuevo.nombre, "0") == 0)
        return;

    // --- Email ---
    solicitarString(
        "Ingrese email del cliente (o CANCELAR): ",
        nuevo.email,
        100,
        emailValido,
        "ERROR: El email debe contener '@' y un dominio.\n"
    );

    if (strcmp(nuevo.email, "CANCELAR") == 0 || strcmp(nuevo.email, "0") == 0)
        return;

    // --- Teléfono ---
    solicitarString("Ingrese teléfono del cliente (o CANCELAR): ",
                    nuevo.telefono, 20);

    if (strcmp(nuevo.telefono, "CANCELAR") == 0 || strcmp(nuevo.telefono, "0") == 0)
        return;

    // --- Dirección ---
    solicitarString("Ingrese dirección del cliente (o CANCELAR): ",
                    nuevo.direccion, 200);

    if (strcmp(nuevo.direccion, "CANCELAR") == 0 || strcmp(nuevo.direccion, "0") == 0)
        return;

    // --- Fecha automática ---
    obtenerFechaActual(nuevo.fechaRegistro);

    // --- Resumen ---
    cout << "\n=== RESUMEN DEL CLIENTE ===\n";
    cout << "Cédula/RIF: " << nuevo.cedula << endl;
    cout << "Nombre: " << nuevo.nombre << endl;
    cout << "Email: " << nuevo.email << endl;
    cout << "Teléfono: " << nuevo.telefono << endl;
    cout << "Dirección: " << nuevo.direccion << endl;

    if (!confirmar("¿Guardar cliente? (S/N): "))
        return;

    // --- Redimensionar si es necesario ---
    if (tienda->numClientes >= tienda->capacidadClientes)
        redimensionarClientes(tienda);

    // --- Guardar ---
    nuevo.id = tienda->siguienteIdCliente++;
    tienda->clientes[tienda->numClientes++] = nuevo;

    cout << "Cliente registrado exitosamente.\n";
}

//2.4.2

void buscarCliente(Tienda* tienda) {
    int opcion;

    do {
        cout << "\n=== BUSCAR CLIENTE ===\n";
        cout << "1. Buscar por ID\n";
        cout << "2. Buscar por nombre (coincidencia parcial)\n";
        cout << "3. Buscar por cédula/RIF\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcion == 0)
            return;

        int index = -1;

        switch (opcion) {

            case 1: {
                int id;
                cout << "Ingrese ID del cliente: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                index = buscarClientePorID(tienda, id);
                break;
            }

            case 2: {
                char nombre[100];
                solicitarString("Ingrese nombre o parte del nombre: ", nombre, 100);
                index = buscarClientePorNombre(tienda, nombre);
                break;
            }

            case 3: {
                char cedula[20];
                solicitarString("Ingrese cédula/RIF del cliente: ", cedula, 20);
                index = buscarClientePorCedula(tienda, cedula);
                break;
            }

            default:
                cout << "Opción inválida.\n";
                continue;
        }

        if (index == -1) {
            cout << "No se encontró ningún cliente con ese criterio.\n";
        } else {
            mostrarCliente(tienda->clientes[index]);
        }

        system("pause");

    } while (opcion != 0);
}

//2.4.3

void actualizarCliente(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del cliente a actualizar (0 para cancelar): ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (id == 0)
        return;

    int index = buscarClientePorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un cliente con ese ID.\n";
        return;
    }

    Cliente& original = tienda->clientes[index];
    Cliente temp = original; // copia temporal

    cout << "\n=== CLIENTE ENCONTRADO ===\n";
    mostrarCliente(original);

    if (!confirmar("\n¿Desea modificar este cliente? (S/N): "))
        return;

    // --- Nombre ---
    cout << "\nNombre actual: " << temp.nombre << endl;
    solicitarString("Nuevo nombre (ENTER para mantener): ", temp.nombre, 100);
    if (strlen(temp.nombre) == 0)
        strcpy(temp.nombre, original.nombre);

    // --- Cédula/RIF ---
    cout << "\nCédula/RIF actual: " << temp.cedula << endl;
    solicitarString("Nueva cédula/RIF (ENTER para mantener): ", temp.cedula, 20);
    if (strlen(temp.cedula) == 0)
        strcpy(temp.cedula, original.cedula);
    else if (clienteDuplicado(tienda, temp.cedula, original.id)) {
        cout << "ERROR: La cédula/RIF '" << temp.cedula << "' ya está registrada.\n";
        return;
    }

    // --- Teléfono ---
    cout << "\nTeléfono actual: " << temp.telefono << endl;
    solicitarString("Nuevo teléfono (ENTER para mantener): ", temp.telefono, 20);
    if (strlen(temp.telefono) == 0)
        strcpy(temp.telefono, original.telefono);

    // --- Email ---
    cout << "\nEmail actual: " << temp.email << endl;
    solicitarString("Nuevo email (ENTER para mantener): ", temp.email, 100);
    if (strlen(temp.email) == 0)
        strcpy(temp.email, original.email);
    else if (!emailValido(temp.email)) {
        cout << "ERROR: El email debe contener '@' y un dominio.\n";
        return;
    }

    // --- Dirección ---
    cout << "\nDirección actual: " << temp.direccion << endl;
    solicitarString("Nueva dirección (ENTER para mantener): ", temp.direccion, 200);
    if (strlen(temp.direccion) == 0)
        strcpy(temp.direccion, original.direccion);

    // --- Confirmación final ---
    cout << "\n=== NUEVOS DATOS DEL CLIENTE ===\n";
    mostrarCliente(temp);

    if (!confirmar("¿Guardar cambios? (S/N): "))
        return;

    original = temp;

    cout << "Cliente actualizado exitosamente.\n";
}


//main temporal

int main() {
    setlocale(LC_ALL, "Spanish");

    Tienda tienda;

    // Inicialización básica
    tienda.numProductos = 0;
    tienda.capacidadProductos = 10;
    tienda.productos = new Producto[tienda.capacidadProductos];
    tienda.siguienteIdProducto = 1;

    tienda.numProveedores = 0;
    tienda.capacidadProveedores = 10;
    tienda.proveedores = new Proveedor[tienda.capacidadProveedores];
    tienda.siguienteIdProveedor = 1;

    tienda.numClientes = 0;
    tienda.capacidadClientes = 10;
    tienda.clientes = new Cliente[tienda.capacidadClientes];
    tienda.siguienteIdCliente = 1;

    tienda.numTransacciones = 0;
    tienda.capacidadTransacciones = 10;
    tienda.transacciones = new Transaccion[tienda.capacidadTransacciones];
    tienda.siguienteIdTransaccion = 1;

    int opcion;

    do {
        system("cls");
        cout << "\n=== MENU PRINCIPAL (TEMPORAL) ===\n";
        cout << "1. Crear producto\n";
        cout << "2. Buscar producto\n";
        cout << "3. Actualizar producto\n";
        cout << "4. Actualizar stock\n";
        cout << "5. Eliminar producto\n";
        cout << "6. Listar productos\n";
        cout << "-------------------------------\n";
        cout << "7. Crear proveedor\n";
        cout << "8. Buscar proveedor\n";
        cout << "9. Actualizar proveedor\n";
        cout << "10. Eliminar proveedor\n";
        cout << "11. Listar proveedores\n";
        cout << "-------------------------------\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        system("cls");

        switch (opcion) {
            // Productos
            case 1: crearProducto(&tienda); break;
            case 2: buscarProducto(&tienda); break;
            case 3: actualizarProducto(&tienda); break;
            case 4: actualizarStockProducto(&tienda); break;
            case 5: eliminarProducto(&tienda); break;
            case 6: listarProductos(&tienda); break;

            // Proveedores
            case 7: crearProveedor(&tienda); break;
            case 8: buscarProveedor(&tienda); break;
            case 9: actualizarProveedor(&tienda); break;
            case 10: eliminarProveedor(&tienda); break;
            case 11: listarProveedores(&tienda); break;

            case 0:
                cout << "Saliendo...\n";
                break;

            default:
                cout << "Opción inválida.\n";
        }

        if (opcion != 0) {
            cout << "\n";
            system("pause");
        }

    } while (opcion != 0);

    // Liberar memoria
    liberarTienda(&tienda);

    return 0;
}


