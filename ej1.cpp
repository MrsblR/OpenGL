
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

const GLdouble dosPi = 6.283185;

/**
 * Clase puntoPantalla: Representa un punto en la pantalla con coordenadas (x, y).
 */
class puntoPantalla
{
public:
    GLint x, y; // Coordenadas x e y del punto

public:
    /**
     * Constructor por defecto: inicializa la posición de las coordenadas en (0, 0).
     */
    puntoPantalla() {
        x = y = 0;
    }

    /**
     * Establece las coordenadas del punto.
     * @param xCoordValor Coordenada x.
     * @param yCoordValor Coordenada y.
     */
    void establecerCoordenadas(GLint xCoordValor, GLint yCoordValor) {
        x = xCoordValor;
        y = yCoordValor;
    }

    /**
     * Obtiene la coordenada x del punto.
     * @return Coordenada x.
     */
    GLint obtenerX() const {
        return x;
    }

    /**
     * Obtiene la coordenada y del punto.
     * @return Coordenada y.
     */
    GLint obtenerY() const {
        return y;
    }

    /**
     * Incrementa la coordenada x en 1.
     */
    void incrementarX() {
        x++;
    }

    /**
     * Decrementa la coordenada y en 1.
     */
    void decrementarY() {
        y--;
    }
};

/**
 * Establece un píxel en las coordenadas especificadas.
 * @param xCoord Coordenada x.
 * @param yCoord Coordenada y.
 */
void establecerPixel(GLint xCoord, GLint yCoord)
{
    glBegin(GL_POINTS);
    glVertex2i(xCoord, yCoord);
    glEnd();
}

/**
 * Dibuja una circunferencia usando el algoritmo de punto medio.
 * @param xc Coordenada x del centro de la circunferencia.
 * @param yc Coordenada y del centro de la circunferencia.
 * @param radio Radio de la circunferencia.
 */
void circunferenciaPuntoMedio(GLint xc, GLint yc, GLint radio)
{
    puntoPantalla puntoCirc;

    // Valor inicial del parámetro de punto medio.
    GLint p = 1 - radio;

    // Establecer coordenadas para el punto superior de la circunferencia.
    puntoCirc.establecerCoordenadas(0, radio);

    void dibujarPuntosCircunferencia(GLint, GLint, puntoPantalla);

    // Dibujar el punto inicial en cada cuadrante de la circunferencia.
    dibujarPuntosCircunferencia(xc, yc, puntoCirc);

    // Calcular el siguiente punto y dibujarlo en cada octante.
    while (puntoCirc.obtenerX() < puntoCirc.obtenerY()) {
        puntoCirc.incrementarX();
        if (p < 0)
            p += 2 * puntoCirc.obtenerX() + 1;
        else {
            puntoCirc.decrementarY();
            p += 2 * (puntoCirc.obtenerX() - puntoCirc.obtenerY()) + 1;
        }
        dibujarPuntosCircunferencia(xc, yc, puntoCirc);
    }
}

/**
 * Dibuja los puntos de la circunferencia en cada uno de los ocho octantes.
 * @param xc Coordenada x del centro de la circunferencia.
 * @param yc Coordenada y del centro de la circunferencia.
 * @param puntoCirc Punto de la circunferencia a dibujar.
 */
void dibujarPuntosCircunferencia(GLint xc, GLint yc, puntoPantalla puntoCirc)
{
    establecerPixel(xc + puntoCirc.obtenerX(), yc + puntoCirc.obtenerY());
    establecerPixel(xc - puntoCirc.obtenerX(), yc + puntoCirc.obtenerY());
    establecerPixel(xc + puntoCirc.obtenerX(), yc - puntoCirc.obtenerY());
    establecerPixel(xc - puntoCirc.obtenerX(), yc - puntoCirc.obtenerY());
    establecerPixel(xc + puntoCirc.obtenerY(), yc + puntoCirc.obtenerX());
    establecerPixel(xc - puntoCirc.obtenerY(), yc + puntoCirc.obtenerX());
    establecerPixel(xc + puntoCirc.obtenerY(), yc - puntoCirc.obtenerX());
    establecerPixel(xc - puntoCirc.obtenerY(), yc - puntoCirc.obtenerX());
}

GLsizei anchoVentana = 400, altoVentana = 300;  // Tamaño inicial de la ventana de visualización.

/**
 * Inicializa el estado de la ventana de visualización.
 */
void iniciar(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);  // Establecer color de fondo blanco.
    glMatrixMode(GL_PROJECTION);  // Establecer modo de proyección.
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);  // Establecer coordenadas del sistema de referencia.
}

/**
 * Dibuja un gráfico circular (gráfico de pastel).
 */
void graficoCircular(void)
{
    puntoPantalla centroCirc, puntoPie;
    GLint radio = anchoVentana / 4;  // Radio de la circunferencia.

    GLdouble anguloSector, anguloSectorAnterior = 0.0;

    GLint k, nSectores = 12;  // Número de sectores.
    GLfloat valoresDatos[12] = { 10.0, 7.0, 13.0, 5.0, 13.0, 14.0,
                                 3.0, 16.0, 5.0, 3.0, 17.0, 8.0 };
    GLfloat sumaDatos = 0.0;

    // Posición del centro de la circunferencia.
    centroCirc.x = anchoVentana / 2;
    centroCirc.y = altoVentana / 2;
    // Llamada a la rutina de dibujo de circunferencia por punto medio.
    circunferenciaPuntoMedio(centroCirc.x, centroCirc.y, radio);

    // Calcular la suma de todos los valores de datos.
    for (k = 0; k < nSectores; k++)
        sumaDatos += valoresDatos[k];

    // Dibujar cada sector del gráfico circular.
    for (k = 0; k < nSectores; k++) {
        anguloSector = dosPi * valoresDatos[k] / sumaDatos + anguloSectorAnterior;
        puntoPie.x = centroCirc.x + radio * cos(anguloSector);
        puntoPie.y = centroCirc.y + radio * sin(anguloSector);
        glBegin(GL_LINES);
        glVertex2i(centroCirc.x, centroCirc.y);
        glVertex2i(puntoPie.x, puntoPie.y);
        glEnd();
        anguloSectorAnterior = anguloSector;
    }
}

/**
 * Función de visualización: se llama para dibujar en la ventana.
 */
void funcionVisualizacion(void)
{
    glClear(GL_COLOR_BUFFER_BIT);  // Limpiar ventana de visualización.

    glColor3f(0.0, 1.0, 0.0);  

    graficoCircular();  // Dibujar el gráfico circular.
    glFlush();
}

/**
 * Ajusta la ventana cuando se redimensiona.
 * @param nuevoAncho Nuevo ancho de la ventana.
 * @param nuevoAlto Nuevo alto de la ventana.
 */
void ajustarVentana(GLint nuevoAncho, GLint nuevoAlto)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, GLdouble(nuevoAncho), 0.0, GLdouble(nuevoAlto));

    glClear(GL_COLOR_BUFFER_BIT);  // Limpiar la ventana de visualización.

    // Restablecer los parámetros de tamaño de la ventana de visualización.
    anchoVentana = nuevoAncho;
    altoVentana = nuevoAlto;
}

/**
 * Función principal: punto de entrada del programa.
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Lista de argumentos de línea de comandos.
 * @return Código de salida del programa.
 */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // Modo de visualización.
    glutInitWindowPosition(100, 100);  // Posición inicial de la ventana.
    glutInitWindowSize(anchoVentana, altoVentana);  // Tamaño inicial de la ventana.
    glutCreateWindow("Gráfico Circular");  // Crear ventana con título.

    iniciar();  // Inicializar estado de la ventana.
    glutDisplayFunc(funcionVisualizacion);  // Registrar función de visualización.
    glutReshapeFunc(ajustarVentana);  // Registrar función de redimensionamiento.

    glutMainLoop();  // Entrar en el bucle principal de GLUT.
    return 0;
}
