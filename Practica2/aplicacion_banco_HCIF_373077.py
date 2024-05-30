import os

class Cuenta:
    def __init__(self,numero_cuenta, titular, cantidad):
        self.numero_cuenta = numero_cuenta
        self.titular = titular
        self.cantidad = cantidad
        
    def depositar(self,cantidad):
        self.cantidad += cantidad
        print(f"Deposito exitoso, saldo actual: {self.cantidad}")
    
    def retirar(self,cantidad):
        if self.cantidad < cantidad:
            print("Saldo insuficiente")
        else:
            self.cantidad -= cantidad
            print(f"Retiro exitoso, saldo actual: {self.cantidad}")
            
    def transferir(self,destino,monto):
        if self.cantidad < monto:
            print("Saldo insuficiente")
        else:
            self.cantidad -= monto
            destino.cantidad += monto
            print(f"Transferencia exitosa, saldo actual: {self.cantidad} \n")
    
    def __str__(self):
        return f"Numero de cuenta: {self.numero_cuenta}\nTitular: {self.titular}\nCantidad: {self.cantidad}"
    
class CuentaAhorros(Cuenta):
    def __init__(self, numero_cuenta, titular, cantidad, interes):
        super().__init__(numero_cuenta, titular, cantidad)
        self.interes = interes
        
    
    def calcular_interes(self):
        self.cantidad += self.cantidad * self.interes
        return self.cantidad
    
    def __str__(self):
        informacion_base = super().__str__()
        return f"{informacion_base}\nInteres: {self.interes} \nSaldo con intereses: {self.calcular_interes()}"
    
class Banco:
    def __init__(self,nombre):
        self.nombre = nombre
        self.cuentas = []
        self.numero_cuentas = 0
        
    def crear_cuenta(self,titular,cantidad):
        self.numero_cuentas += 1
        nueva_cuenta = Cuenta(self.numero_cuentas,titular,cantidad)
        self.cuentas.append(nueva_cuenta)
        print(f"Cuenta creada con exito, numero de cuenta: {self.numero_cuentas}")
        return nueva_cuenta
    
    def crear_cuenta_ahorros(self,titular,cantidad,interes):
        self.numero_cuentas += 1
        nueva_cuenta = CuentaAhorros(self.numero_cuentas,titular,cantidad,interes)
        self.cuentas.append(nueva_cuenta)
        return nueva_cuenta
    
    def getCuenta(self,numero_cuenta):
        for cuenta in self.cuentas:
            if cuenta.numero_cuenta == numero_cuenta:
                return cuenta
        return None
    
    def __str__(self):
        result = f"Banco: {self.nombre}\n"
        for cuenta in self.cuentas:
            result += str(cuenta) + "\n"
            result += "----------------------------\n"
        return result
            

banco = Banco("Banca Grupo HERCE")

def menu():
    print("Bienvenido al Banco")
    print("1. Crear cuenta")
    print("2. Depositar")
    print("3. Retirar")
    print("4. Transferir")
    print("5. Consultar saldo")
    print("6. Imprimir todas las cuentas")
    print("7. Imprimir una cuenta")
    print("8. Salir")

    opcion = input("Ingrese el numero de la opcion que desea realizar: ")
    
    return opcion

def crear_cuenta():
    tipo_cuenta = input("Ingrese el tipo de cuenta que desea crear (1. Cuenta normal, 2. Cuenta de ahorros): ")
    if tipo_cuenta == "1":
        titular = input("Ingrese el nombre del titular: ")
        cantidad = float(input("Ingrese la cantidad inicial: "))
        cuenta = banco.crear_cuenta(titular, cantidad)
    elif tipo_cuenta == "2":
        interes = input("Ingrese el interes de la cuenta de ahorros 1 - [0.5] 2 - [0.1] 3 - [0.05]: ")
        if interes == "1":
            interes = 0.5
        elif interes == "2":
            interes = 0.1
        elif interes == "3":
            interes = 0.05
        titular = input("Ingrese el nombre del titular: ")
        cantidad = float(input("Ingrese la cantidad inicial: "))
        cuenta = banco.crear_cuenta_ahorros(titular, cantidad, interes)
    else:
        print("Opcion invalida")

def Depositar():
    numero_cuenta = int(input("Ingrese el numero de cuenta: "))
    cuenta = banco.getCuenta(numero_cuenta)
    if cuenta:
        cantidad = float(input("Ingrese la cantidad a depositar: "))
        cuenta.depositar(cantidad)
    else:
        print("Cuenta no encontrada")

def Retirar():
    numero_cuenta = int(input("Ingrese el numero de cuenta: "))
    cuenta = banco.getCuenta(numero_cuenta)
    if cuenta:
        cantidad = float(input("Ingrese la cantidad a retirar: "))
        cuenta.retirar(cantidad)
    else:
        print("Cuenta no encontrada")

def Transferir():
    numero_cuenta_origen = int(input("Ingresa el numero de cuenta Emisora: "))
    cuenta_origen = banco.getCuenta(numero_cuenta_origen)
    if cuenta_origen:
        numero_cuenta_destino = int(input("Ingresa el numero de cuenta Receptora: "))
        cuenta_destino = banco.getCuenta(numero_cuenta_destino)
        if cuenta_destino == cuenta_origen:
            print("No puedes transferir a la misma cuenta.")
        elif cuenta_destino:
                cantidad = float(input("Ingresa la cantidad de la transferencia: "))
                cuenta_origen.transferir(cuenta_destino, cantidad)
        else:
                print("La cuenta emisora no se encontro en el sistema.")
    else:
        print("La cuenta Receptora no se encontro en el sistema.")

def Consultar_saldo():
    numero_cuenta = int(input("Ingrese el numero de cuenta: "))
    cuenta = banco.getCuenta(numero_cuenta)
    if cuenta:    
        print(cuenta)

def Imprimir_cuentas():
    print(banco)

def Imprimir_cuenta():
    numero_cuenta = int(input("Ingrese el numero de cuenta: "))
    cuenta = banco.getCuenta(numero_cuenta)
    if cuenta:
        print(cuenta)
    else:
        print("El numero de cuenta no se encontro en el sistema.")

def main():
    sistema = True
    while sistema: 
        print("Bienvenido a la banca Grupo HERCE")
        opcion = menu()
        
        if opcion == "1":
            crear_cuenta()
            
        elif opcion == "2":
            Depositar()
        
        elif opcion == "3":
            Retirar()
        
        elif opcion == "4":
            Transferir()
        
        elif opcion == "5":
            Consultar_saldo()

        elif opcion == "6":
            Imprimir_cuentas()
        
        elif opcion == "7":
            Imprimir_cuenta()
        
        elif opcion == "8":
            print("Gracias por confiar en Grupo HERCE. Hasta luego.")
            sistema = False
        
        else:
            print("Opcion invalida. Por favor, ingrese una opcion valida.")
        
        input("Presione cualquier tecla para continuar...")
        os.system("cls")
        
main()

