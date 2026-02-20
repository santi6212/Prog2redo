#include <iostream>
#include <ctime> 
#include <cctype>
#include <cstring>
#include <iomanip>


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

//verificaciones
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


//2.1 inicializar
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

//2.2.1

void crearProducto(Tienda* tienda) {
    char opcion[10];
    cout << "¿Desea registrar un nuevo producto? (S/N): ";
    cin >> opcion;

    if (toupper(opcion[0]) != 'S')
        return;

    Producto nuevo;

    // --- Código ---
    cout << "Ingrese código del producto (o CANCELAR): ";
    cin >> nuevo.codigo;
    if (strcmp(nuevo.codigo, "CANCELAR") == 0 || strcmp(nuevo.codigo, "0") == 0)
        return;

    if (codigoProductoDuplicado(tienda, nuevo.codigo)) {
        cout << "ERROR: Código duplicado.\n";
        return;
    }

    // --- Nombre ---
    cout << "Ingrese nombre del producto (o CANCELAR): ";
    cin.ignore();
    cin.getline(nuevo.nombre, 100);
    if (strcmp(nuevo.nombre, "CANCELAR") == 0 || strcmp(nuevo.nombre, "0") == 0)
        return;

    // --- Descripción ---
    cout << "Ingrese descripción (o CANCELAR): ";
    cin.getline(nuevo.descripcion, 200);
    if (strcmp(nuevo.descripcion, "CANCELAR") == 0 || strcmp(nuevo.descripcion, "0") == 0)
        return;

    // --- ID Proveedor ---
    cout << "Ingrese ID del proveedor (o CANCELAR): ";
    cin >> nuevo.idProveedor;
    if (nuevo.idProveedor == 0)
        return;

    if (!existeProveedor(tienda, nuevo.idProveedor)) {
        cout << "ERROR: El proveedor no existe.\n";
        return;
    }

    // --- Precio ---
    cout << "Ingrese precio (>0): ";
    cin >> nuevo.precio;
    if (nuevo.precio <= 0) {
        cout << "ERROR: Precio inválido.\n";
        return;
    }

    // --- Stock ---
    cout << "Ingrese stock (>=0): ";
    cin >> nuevo.stock;
    if (nuevo.stock < 0) {
        cout << "ERROR: Stock inválido.\n";
        return;
    }

    obtenerFechaActual(nuevo.fechaRegistro);

    cout << "\n=== RESUMEN DEL PRODUCTO ===\n";
    cout << "Código: " << nuevo.codigo << endl;
    cout << "Nombre: " << nuevo.nombre << endl;
    cout << "Descripción: " << nuevo.descripcion << endl;
    cout << "Proveedor ID: " << nuevo.idProveedor << endl;
    cout << "Precio: " << nuevo.precio << endl;
    cout << "Stock: " << nuevo.stock << endl;
    cout << "Fecha: " << nuevo.fechaRegistro << endl;

    cout << "\n¿Guardar producto? (S/N): ";
    cin >> opcion;
    if (toupper(opcion[0]) != 'S')
        return;

    // --- Redimensionar si es necesario ---
    if (tienda->numProductos >= tienda->capacidadProductos) {
        int nuevaCap = tienda->capacidadProductos * 2;
        Producto* nuevoArray = new Producto[nuevaCap];

        for (int i = 0; i < tienda->numProductos; i++)
            nuevoArray[i] = tienda->productos[i];

        delete[] tienda->productos;
        tienda->productos = nuevoArray;
        tienda->capacidadProductos = nuevaCap;
    }

    // guardado
    nuevo.id = tienda->siguienteIdProducto++;
    tienda->productos[tienda->numProductos++] = nuevo;

    cout << "\nProducto registrado exitosamente.\n";
}

//2.2.2

//buscar
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

    // Para búsquedas parciales
    char buffer[200];
    string filtro;

    switch (opcion) {
    	
    // 1. Buscar por ID
    
    case 1: {
        int id;
        cout << "Ingrese ID del producto: ";
        cin >> id;

        for (int i = 0; i < tienda->numProductos; i++) {
            if (tienda->productos[i].id == id) {
                Producto& p = tienda->productos[i];
                cout << "\n=== PRODUCTO ENCONTRADO ===\n";
                cout << "ID: " << p.id << endl;
                cout << "Código: " << p.codigo << endl;
                cout << "Nombre: " << p.nombre << endl;
                cout << "Proveedor: " << p.idProveedor << endl;
                cout << "Precio: " << p.precio << endl;
                cout << "Stock: " << p.stock << endl;
                cout << "Fecha: " << p.fechaRegistro << endl;
                return;
            }
        }

        cout << "No se encontró un producto con ese ID.\n";
        return;
    }

    // 2. Buscar por nombre (parcial)
    
    case 2: {
        cout << "Ingrese parte del nombre: ";
        cin.ignore();
        cin.getline(buffer, 200);
        filtro = toLower(buffer);

        bool encontrado = false;

        cout << "\n=== RESULTADOS ===\n";
        for (int i = 0; i < tienda->numProductos; i++) {
            if (contiene(tienda->productos[i].nombre, filtro)) {
                Producto& p = tienda->productos[i];
                cout << "ID: " << p.id
                     << " | " << p.codigo
                     << " | " << p.nombre
                     << " | Precio: " << p.precio
                     << " | Stock: " << p.stock << endl;
                encontrado = true;
            }
        }

        if (!encontrado)
            cout << "No se encontraron coincidencias.\n";

        return;
    }

    // 3. Buscar por código (parcial)
    
    case 3: {
        cout << "Ingrese parte del código: ";
        cin.ignore();
        cin.getline(buffer, 200);
        filtro = toLower(buffer);

        bool encontrado = false;

        cout << "\n=== RESULTADOS ===\n";
        for (int i = 0; i < tienda->numProductos; i++) {
            if (contiene(tienda->productos[i].codigo, filtro)) {
                Producto& p = tienda->productos[i];
                cout << "ID: " << p.id
                     << " | " << p.codigo
                     << " | " << p.nombre
                     << " | Precio: " << p.precio
                     << " | Stock: " << p.stock << endl;
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

        bool encontrado = false;

        cout << "\n=== PRODUCTOS DEL PROVEEDOR " << idProv << " ===\n";
        for (int i = 0; i < tienda->numProductos; i++) {
            if (tienda->productos[i].idProveedor == idProv) {
                Producto& p = tienda->productos[i];
                cout << "ID: " << p.id
                     << " | " << p.codigo
                     << " | " << p.nombre
                     << " | Precio: " << p.precio
                     << " | Stock: " << p.stock << endl;
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

//2.2.3
void actualizarProducto(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del producto a actualizar: ";
    cin >> id;

    // Buscar producto
    Producto* p = nullptr;
    for (int i = 0; i < tienda->numProductos; i++) {
        if (tienda->productos[i].id == id) {
            p = &tienda->productos[i];
            break;
        }
    }

    if (!p) {
        cout << "ERROR: No existe un producto con ese ID.\n";
        return;
    }

    // Copia temporal para editar sin afectar el original
    Producto temp = *p;

    cout << "\n=== DATOS ACTUALES ===\n";
    cout << "1. Código: " << temp.codigo << endl;
    cout << "2. Nombre: " << temp.nombre << endl;
    cout << "3. Descripción: " << temp.descripcion << endl;
    cout << "4. Proveedor ID: " << temp.idProveedor << endl;
    cout << "5. Precio: " << temp.precio << endl;
    cout << "6. Stock: " << temp.stock << endl;

    int opcion;
    char buffer[200];

    do {
        cout << "\n¿Qué desea editar?\n";
        cout << "1. Código\n";
        cout << "2. Nombre\n";
        cout << "3. Descripción\n";
        cout << "4. Proveedor\n";
        cout << "5. Precio\n";
        cout << "6. Stock\n";
        cout << "7. Guardar cambios\n";
        cout << "0. Cancelar sin guardar\n";
        cout << "Opción: ";
        cin >> opcion;

        cin.ignore();

        switch (opcion) {

        case 1: // Código
            cout << "Nuevo código: ";
            cin.getline(buffer, 20);

            if (codigoProductoDuplicado(tienda, buffer, temp.id)) {
                cout << "ERROR: Código duplicado.\n";
            } else {
                strcpy(temp.codigo, buffer);
            }
            break;

        case 2: // Nombre
            cout << "Nuevo nombre: ";
            cin.getline(temp.nombre, 100);
            break;

        case 3: // Descripción
            cout << "Nueva descripción: ";
            cin.getline(temp.descripcion, 200);
            break;

        case 4: { // Proveedor
            int nuevoProv;
            cout << "Nuevo ID de proveedor: ";
            cin >> nuevoProv;

            if (!existeProveedor(tienda, nuevoProv)) {
                cout << "ERROR: El proveedor no existe.\n";
            } else {
                temp.idProveedor = nuevoProv;
            }
            break;
        }

        case 5: // Precio
            cout << "Nuevo precio (>0): ";
            cin >> temp.precio;
            if (temp.precio <= 0) {
                cout << "ERROR: Precio inválido.\n";
            }
            break;

        case 6: // Stock
            cout << "Nuevo stock (>=0): ";
            cin >> temp.stock;
            if (temp.stock < 0) {
                cout << "ERROR: Stock inválido.\n";
            }
            break;

        case 7: { // Guardar cambios
            char confirm[10];
            cout << "\n¿Confirmar cambios? (S/N): ";
            cin >> confirm;

            if (toupper(confirm[0]) == 'S') {
                *p = temp;
                cout << "Cambios guardados exitosamente.\n";
            } else {
                cout << "Cambios descartados.\n";
            }
            return;
        }

        case 0:
            cout << "Cancelado. No se guardaron cambios.\n";
            return;

        default:
            cout << "Opción inválida.\n";
        }

    } while (true);
}

//2.2.4
void actualizarStockProducto(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del producto: ";
    cin >> id;

    // Buscar producto
    Producto* p = nullptr;
    for (int i = 0; i < tienda->numProductos; i++) {
        if (tienda->productos[i].id == id) {
            p = &tienda->productos[i];
            break;
        }
    }

    if (!p) {
        cout << "ERROR: No existe un producto con ese ID.\n";
        return;
    }

    cout << "\n=== ACTUALIZAR STOCK ===\n";
    cout << "Producto: " << p->nombre << endl;
    cout << "Stock actual: " << p->stock << endl;

    int ajuste;
    cout << "Ingrese ajuste (+ para aumentar, - para disminuir): ";
    cin >> ajuste;

    int nuevoStock = p->stock + ajuste;

    if (nuevoStock < 0) {
        cout << "ERROR: El stock no puede quedar negativo.\n";
        return;
    }

    cout << "Stock final sería: " << nuevoStock << endl;

    char confirm[10];
    cout << "¿Confirmar cambio? (S/N): ";
    cin >> confirm;

    if (toupper(confirm[0]) == 'S') {
        p->stock = nuevoStock;
        cout << "Stock actualizado exitosamente.\n";
    } else {
        cout << "Cambio cancelado.\n";
    }
}

//2.2.5
void listarProductos(Tienda* tienda) {
    if (tienda->numProductos == 0) {
        cout << "No hay productos registrados.\n";
        return;
    }

    cout << "\n================ LISTA DE PRODUCTOS ================\n";
    cout << left
         << setw(5)  << "ID"
         << setw(12) << "Código"
         << setw(20) << "Nombre"
         << setw(20) << "Proveedor"
         << setw(10) << "Precio"
         << setw(8)  << "Stock"
         << endl;

    cout << "---------------------------------------------------------------\n";

    for (int i = 0; i < tienda->numProductos; i++) {
        Producto& p = tienda->productos[i];

        const char* nombreProv = obtenerNombreProveedor(tienda, p.idProveedor);

        cout << left
             << setw(5)  << p.id
             << setw(12) << p.codigo
             << setw(20) << p.nombre
             << setw(20) << nombreProv
             << setw(10) << p.precio
             << setw(8)  << p.stock
             << endl;
    }

    cout << "===============================================================\n";
}
//2.2.6
void eliminarProducto(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del producto a eliminar: ";
    cin >> id;

    // Buscar producto
    int index = -1;
    for (int i = 0; i < tienda->numProductos; i++) {
        if (tienda->productos[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "ERROR: No existe un producto con ese ID.\n";
        return;
    }

    Producto& p = tienda->productos[index];

    // Mostrar datos
    cout << "\n=== PRODUCTO A ELIMINAR ===\n";
    cout << "ID: " << p.id << endl;
    cout << "Código: " << p.codigo << endl;
    cout << "Nombre: " << p.nombre << endl;
    cout << "Proveedor: " << obtenerNombreProveedor(tienda, p.idProveedor) << endl;
    cout << "Precio: " << p.precio << endl;
    cout << "Stock: " << p.stock << endl;

    // Advertir si tiene transacciones
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

    // Confirmación
    char confirm[10];
    cout << "\n¿Eliminar producto? (S/N): ";
    cin >> confirm;

    if (toupper(confirm[0]) != 'S') {
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

