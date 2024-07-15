// Inclusión de bibliotecas necesarias
#include <GL/glut.h>  // Para funciones de OpenGL y GLUT
#include <cmath>      // Para funciones matemáticas
#include <algorithm>  // Para std::max
#include <cstdlib>    // Para funciones estándar de C

using namespace std;

// Función DDA (Digital Differential Analyzer) para dibujar líneas
void DDA(int x1, int y1, int x2, int y2, int style) {
    // Cálculo de diferencias y pasos
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = std::max(abs(dx), abs(dy));
    
    // Cálculo de incrementos
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;
    
    // Inicialización de coordenadas
    float x = x1;
    float y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        // Dibujo según el estilo especificado
        if (style == 0) {  // Estilo de puntos
            if (i % 2 == 0) {
                glVertex2i(round(x), round(y));
            }
        }
        else if (style == 1) {  // Estilo de guiones
            if (i % 10 < 5) {
                glVertex2i(round(x), round(y));
            }
        }
        else {  // Línea sólida
            glVertex2i(round(x), round(y));
        }
        // Actualización de coordenadas
        x += xIncrement;
        y += yIncrement;
    }
    glEnd();
    glFlush();
}

// Función de Bresenham para dibujar líneas
void Bresenham(int x1, int y1, int x2, int y2, int style) {
    // Cálculo de diferencias y direcciones
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    
    // Inicialización de error
    int err = dx - dy;
    int e2;

    glBegin(GL_POINTS);
    while (true) {
        // Dibujo según el estilo especificado
        if (style == 0) {  // Estilo de puntos
            glVertex2i(x1, y1);
        }
        else if (style == 1) {  // Estilo de guiones
            if ((x1 + y1) % 10 < 5) {
                glVertex2i(x1, y1);
            }
        }
        else {  // Línea sólida
            glVertex2i(x1, y1);
        }

        // Condición de salida
        if (x1 == x2 && y1 == y2) break;
        
        // Actualización de error y coordenadas
        e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    glEnd();
    glFlush();
}

// Definición de la estructura Casa
struct Casa {
    int x;      // Coordenada x del origen
    int y;      // Coordenada y del origen
    int altura; // Altura de la casa
    int ancho;  // Ancho de la casa
};

// Función para crear y dibujar una casa
void crearCasa(Casa& C) {
    // Dibujo de la base de la casa
    glColor3f(1.0, 0.0, 0.0);  // Color rojo
    DDA(C.x, C.y, C.x + C.ancho, C.y, 2);
    DDA(C.x + C.ancho, C.y, C.x + C.ancho, C.y + C.altura, 2);
    DDA(C.x + C.ancho, C.y + C.altura, C.x, C.y + C.altura, 2);
    DDA(C.x, C.y + C.altura, C.x, C.y, 2);

    // Dibujo del techo
    glColor3f(0.0, 1.0, 0.0);  // Color verde
    DDA(C.x, C.y + C.altura, C.x + C.ancho / 2, C.y + C.altura + C.altura / 2, 2);
    DDA(C.x + C.ancho / 2, C.y + C.altura + C.altura / 2, C.x + C.ancho, C.y + C.altura, 2);

    // Dibujo de la puerta
    glColor3f(0.5, 0.3, 0.0);  // Color marrón
    int puertaAltura = C.altura / 2;
    int puertaAncho = C.ancho / 5;
    int puertaX = C.x + C.ancho / 2 - puertaAncho / 2;
    int puertaY = C.y;
    DDA(puertaX, puertaY, puertaX + puertaAncho, puertaY, 2);
    DDA(puertaX + puertaAncho, puertaY, puertaX + puertaAncho, puertaY + puertaAltura, 2);
    DDA(puertaX + puertaAncho, puertaY + puertaAltura, puertaX, puertaY + puertaAltura, 2);
    DDA(puertaX, puertaY + puertaAltura, puertaX, puertaY, 2);
}

// Función para dibujar una casa con dimensiones predefinidas
void dibujarCasa(void) {
    Casa C;
    C.x = 50;
    C.y = 50;
    C.altura = 100;
    C.ancho = 200;
    crearCasa(C);
}

// Función para dibujar un círculo
void drawCircle(int xc, int yc, int radius) {
    glBegin(GL_POINTS);
    for (int angle = 0; angle <= 360; angle++) {
        float theta = angle * 3.14159 / 180;
        int x = xc + radius * cos(theta);
        int y = yc + radius * sin(theta);
        glVertex2i(x, y);
    }
    glEnd();
}

// Definición de la estructura Carro
struct Carro {
    int x;      // Coordenada x del origen
    int y;      // Coordenada y del origen
    int altura; // Altura del carro
    int ancho;  // Ancho del carro
};

// Función para crear y dibujar un carro
void crearCarro(Carro& C) {
    // Dibujo del cuerpo del carro
    glColor3f(0.0, 0.0, 1.0);  // Color azul
    DDA(C.x, C.y, C.x + C.ancho, C.y, 2);
    DDA(C.x + C.ancho, C.y, C.x + C.ancho, C.y + C.altura, 2);
    DDA(C.x + C.ancho, C.y + C.altura, C.x, C.y + C.altura, 2);
    DDA(C.x, C.y + C.altura, C.x, C.y, 2);

    // Dibujo de la cabina del carro
    glColor3f(1.0, 1.0, 0.0);  // Color amarillo
    int cabinaY = C.y + C.altura;
    DDA(C.x + C.ancho / 4, cabinaY, C.x + 3 * C.ancho / 4, cabinaY, 2);
    DDA(C.x + 3 * C.ancho / 4, cabinaY, C.x + 3 * C.ancho / 4, cabinaY + C.altura / 2, 2);
    DDA(C.x + 3 * C.ancho / 4, cabinaY + C.altura / 2, C.x + C.ancho / 4, cabinaY + C.altura / 2, 2);
    DDA(C.x + C.ancho / 4, cabinaY + C.altura / 2, C.x + C.ancho / 4, cabinaY, 2);

    // Dibujo de las ruedas del carro
    glColor3f(0.0, 1.0, 0.0);  // Color verde
    int radio = C.altura / 4;
    drawCircle(C.x + C.ancho / 4, C.y - radio, radio);
    drawCircle(C.x + 3 * C.ancho / 4, C.y - radio, radio);
}

// Función para dibujar un carro con dimensiones predefinidas
void dibujarCarro(void) {
    Carro C;
    C.x = 300;
    C.y = 50;
    C.altura = 50;
    C.ancho = 100;
    crearCarro(C);
}

// Función para trasladar una casa
void trasladarCasa(Casa& C, int deltaX, int deltaY) {
    C.x += deltaX;
    C.y += deltaY;

    // Limitar la posición dentro de la ventana
    if (C.x < 0) C.x = 0;
    if (C.y < 0) C.y = 0;
    if (C.x + C.ancho > 500) C.x = 500 - C.ancho;
    if (C.y + C.altura > 500) C.y = 500 - C.altura;
}

// Función para escalar una casa
void escalarCasa(Casa& C, float factor) {
    int nuevaAltura = C.altura * factor;
    int nuevoAncho = C.ancho * factor;

    // Limitar el tamaño dentro de la ventana
    if (C.x + nuevoAncho > 500) {
        nuevoAncho = 500 - C.x;
        C.altura = nuevoAncho / factor;
    }
    if (C.y + nuevaAltura > 500) {
        nuevaAltura = 500 - C.y;
        C.ancho = nuevaAltura / factor;
    }

    C.altura = nuevaAltura;
    C.ancho = nuevoAncho;
}

// Función para trasladar un carro
void trasladarCarro(Carro& C, int deltaX, int deltaY) {
    C.x += deltaX;
    C.y += deltaY;

    // Limitar la posición dentro de la ventana
    if (C.x < 0) C.x = 0;
    if (C.y < 0) C.y = 0;
    if (C.x + C.ancho > 500) C.x = 500 - C.ancho;
    if (C.y + C.altura > 500) C.y = 500 - C.altura;
}

// Función para escalar un carro
void escalarCarro(Carro& C, float factor) {
    int nuevaAltura = C.altura * factor;
    int nuevoAncho = C.ancho * factor;

    // Limitar el tamaño dentro de la ventana
    if (C.x + nuevoAncho > 500) {
        nuevoAncho = 500 - C.x;
        C.altura = nuevoAncho / factor;
    }
    if (C.y + nuevaAltura > 500) {
        nuevaAltura = 500 - C.y;
        C.ancho = nuevaAltura / factor;
    }

    C.altura = nuevaAltura;
    C.ancho = nuevoAncho;
}

// Función para dibujar una casa trasladada y escalada
void dibujarCasaTrasladadaYEscalada(void) {
    Casa C;
    C.x = 50;
    C.y = 300;
    C.altura = 150;
    C.ancho = 300;
    trasladarCasa(C, 50, 50);
    escalarCasa(C, 1.5);
    crearCasa(C);
}

// Función para dibujar un carro trasladado y escalado
void dibujarCarroTrasladadoYEscalado(void) {
    Carro C;
    C.x = 300;
    C.y = 300;
    C.altura = 75;
    C.ancho = 150;
    trasladarCarro(C, 50, 50);
    escalarCarro(C, 1.5);
    crearCarro(C);
}

// Función principal de visualización
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Limpia el buffer de color
    dibujarCasa();
    dibujarCarro();
    dibujarCasaTrasladadaYEscalada();
    dibujarCarroTrasladadoYEscalado();
    glFlush();  // Fuerza el dibujado de todos los comandos OpenGL pendientes
}

// Función principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);  // Inicializa GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // Configura el modo de visualización
    glutInitWindowSize(700, 700);  // Establece el tamaño de la ventana
    glutCreateWindow("Casa y Carro");  // Crea la ventana con un título
    gluOrtho2D(0, 500, 0, 500);  // Establece el sistema de coordenadas
    glutDisplayFunc(display);  // Registra la función de visualización
    glutMainLoop();  // Inicia el bucle principal de GLUT
    return 0;
}
