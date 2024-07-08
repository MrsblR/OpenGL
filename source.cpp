#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include <cstdlib>

using namespace std;

void DDA(int x1, int y1, int x2, int y2, int style) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = std::max(abs(dx), abs(dy));
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;
    float x = x1;
    float y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        if (style == 0) { // Dibujar puntos
            if (i % 2 == 0) {
                glVertex2i(round(x), round(y));
            }
        }
        else if (style == 1) { // Dibujar guiones
            if (i % 10 < 5) {
                glVertex2i(round(x), round(y));
            }
        }
        else { // Dibujar línea sólida
            glVertex2i(round(x), round(y));
        }
        x += xIncrement;
        y += yIncrement;
    }
    glEnd();
    glFlush();
}

void Bresenham(int x1, int y1, int x2, int y2, int style) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int e2;

    glBegin(GL_POINTS);
    while (true) {
        if (style == 0) { // Dibujar puntos
            glVertex2i(x1, y1);
        }
        else if (style == 1) { // Dibujar guiones
            if ((x1 + y1) % 10 < 5) {
                glVertex2i(x1, y1);
            }
        }
        else { // Dibujar línea sólida
            glVertex2i(x1, y1);
        }

        if (x1 == x2 && y1 == y2) break;
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


struct Casa {
    int x; // origen en x
    int y; // origen en y
    int altura;
    int ancho;
};

void crearCasa(Casa& C) {
    // Dibujar base de la casa
    glColor3f(1.0, 0.0, 0.0);
    DDA(C.x, C.y, C.x + C.ancho, C.y, 2);
    DDA(C.x + C.ancho, C.y, C.x + C.ancho, C.y + C.altura, 2);
    DDA(C.x + C.ancho, C.y + C.altura, C.x, C.y + C.altura, 2);
    DDA(C.x, C.y + C.altura, C.x, C.y, 2);

    // Dibujar techo de la casa
    glColor3f(0.0, 1.0, 0.0);
    DDA(C.x, C.y + C.altura, C.x + C.ancho / 2, C.y + C.altura + C.altura / 2, 2);
    DDA(C.x + C.ancho / 2, C.y + C.altura + C.altura / 2, C.x + C.ancho, C.y + C.altura, 2);


    // Dibujar puerta
    glColor3f(0.5, 0.3, 0.0);
    int puertaAltura = C.altura / 2;
    int puertaAncho = C.ancho / 5;
    int puertaX = C.x + C.ancho / 2 - puertaAncho / 2;
    int puertaY = C.y;
    DDA(puertaX, puertaY, puertaX + puertaAncho, puertaY, 2);
    DDA(puertaX + puertaAncho, puertaY, puertaX + puertaAncho, puertaY + puertaAltura , 2);
    DDA(puertaX + puertaAncho, puertaY + puertaAltura, puertaX, puertaY + puertaAltura, 2);
    DDA(puertaX, puertaY + puertaAltura, puertaX, puertaY, 2);

}

void dibujarCasa(void) {
    Casa C;
    C.x = 100;
    C.y = 100;
    C.altura = 100;
    C.ancho = 200;
    crearCasa(C);
}



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


struct Carro {
    int x; // origen en x
    int y; // origen en y
    int altura;
    int ancho;
};


void crearCarro(Carro& C) {
    // Dibujar cuerpo del carro
    glColor3f(0.0, 0.0, 1.0);
    DDA(C.x, C.y, C.x + C.ancho, C.y, 2);
    DDA(C.x + C.ancho, C.y, C.x + C.ancho, C.y + C.altura, 2);
    DDA(C.x + C.ancho, C.y + C.altura, C.x, C.y + C.altura, 2);
    DDA(C.x, C.y + C.altura, C.x, C.y, 2);

    // Dibujar cabina del carro
    glColor3f(1.0, 1.0, 0.0);
    int cabinaY = C.y + C.altura;
    DDA(C.x + C.ancho / 4, cabinaY, C.x + 3 * C.ancho / 4, cabinaY, 2);
    DDA(C.x + 3 * C.ancho / 4, cabinaY, C.x + 3 * C.ancho / 4, cabinaY + C.altura / 2, 2);
    DDA(C.x + 3 * C.ancho / 4, cabinaY + C.altura / 2, C.x + C.ancho / 4, cabinaY + C.altura / 2, 2);
    DDA(C.x + C.ancho / 4, cabinaY + C.altura / 2, C.x + C.ancho / 4, cabinaY, 2);

    // Dibujar ruedas del carro
    glColor3f(0.0, 1.0, 0.0);
    int radio = C.altura / 4;
    drawCircle(C.x + C.ancho / 4, C.y - radio, radio);
    drawCircle(C.x + 3 * C.ancho / 4, C.y - radio, radio);


}

void dibujarCarro(void) {
    Carro C;
    C.x = 200;
    C.y = 100;
    C.altura = 50;
    C.ancho = 100;
    crearCarro(C);
}

//void display() {
//    glClear(GL_COLOR_BUFFER_BIT);
//    //dibujarCasa();
//    dibujarCarro();
//    glFlush();
//}



void trasladarCasa(Casa& C, int deltaX, int deltaY) {
    C.x += deltaX;
    C.y += deltaY;
}

void escalarCasa(Casa& C, float factor) {
    C.altura *= factor;
    C.ancho *= factor;
}

void trasladarCarro(Carro& C, int deltaX, int deltaY) {
    C.x += deltaX;
    C.y += deltaY;
}

void escalarCarro(Carro& C, float factor) {
    C.altura *= factor;
    C.ancho *= factor;
}

void dibujarCasaTrasladadaYEscalada(void) {
    Casa C;
    C.x = 100;
    C.y = 100;
    C.altura = 100;
    C.ancho = 200;
    trasladarCasa(C, 10, 20);
    escalarCasa(C, 1.5);
    crearCasa(C);
}

void dibujarCarroTrasladadoYEscalado(void) {
    Carro C;
    C.x = 200;
    C.y = 100;
    C.altura = 50;
    C.ancho = 100;
    trasladarCarro(C, 10, 20);
    escalarCarro(C, 1.5);
    crearCarro(C);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    //dibujarCasaTrasladadaYEscalada();
    dibujarCarroTrasladadoYEscalado();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Casa y Carro Trasladados y Escalados");
    gluOrtho2D(0, 500, 0, 500);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}




