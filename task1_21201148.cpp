#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

int x1_, y1_, x2_, y2_;  // Line endpoints (user input)
int lineWidth = 1;       // Line thickness (user input)

void plotPoint(int x, int y) {
    glVertex2i(x, y);
}

// Standard Bresenham
void bresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        plotPoint(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

// 4-way symmetric thick line
void drawThickLine4Way(int x1, int y1, int x2, int y2, int thickness) {
    if (thickness <= 1) {
        glBegin(GL_POINTS);
        bresenhamLine(x1, y1, x2, y2);
        glEnd();
        return;
    }
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int half = thickness / 2;

    glBegin(GL_POINTS);
    while (1) {
        // Cross stroke around (x1,y1): left-right & up-down
        for (int k = -half; k <= half; ++k) {
            plotPoint(x1 + k, y1);   // horizontal spread
            plotPoint(x1,     y1 + k); // vertical spread
        }
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
    glEnd();
}

// Draw "+" axes at window center for coords (0..800, 0..600)
void drawAxes() {
    // Black axes so it's visible on white background
    glColor3f(0.0, 0.0, 0.0);  // <-- If you strictly want white "+", change to (1,1,1)
    glBegin(GL_LINES);
    // Vertical line at x = 400
    glVertex2i(400, 0);   glVertex2i(400, 600);
    // Horizontal line at y = 300
    glVertex2i(0, 300);   glVertex2i(800, 300);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1) Axes first (black "+")
    drawAxes();

    // 2) Standard line (red) at user-given coords
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    bresenhamLine(x1_, y1_, x2_, y2_);
    glEnd();

    // 3) 4-way thick line (blue), shifted right by +200 for side-by-side view
    glColor3f(0.0, 0.0, 1.0);
    drawThickLine4Way(x1_ + 200, y1_, x2_ + 200, y2_, lineWidth);

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background (as you wanted)
    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Keep your original user-input-friendly coordinate system
    gluOrtho2D(0, 800, 0, 600);
}

int main(int argc, char **argv) {
    printf("Enter x1 y1 x2 y2 (e.g., 100 100 400 300): ");
    scanf("%d %d %d %d", &x1_, &y1_, &x2_, &y2_);

    printf("Enter line thickness (e.g., 5): ");
    scanf("%d", &lineWidth);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bresenham + 4-way Thick | White BG + Axes");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
