
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include "raygui.h"
#include <stdlib.h>
#include <stdio.h>
#include "build_points/pnts.h"
#include "build_points/zoom.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

Point *rebuildGraph(double *max, double *min, int *nump, int numpoints, float l, float a) {
    return getGraphAbsolute(numpoints, nump, max, min, l, a);
}

Font font;

void drawText(const char *text, int posX, int posY, int fontSize, Color color) {
    // Check if default font has been loaded
    if (GetFontDefault().texture.id != 0) {
        Vector2 position = {(float) posX, (float) posY};

        int defaultFontSize = 8;   // Default Font chars height in pixel
        if (fontSize < defaultFontSize) fontSize = defaultFontSize;
        int spacing = fontSize / defaultFontSize;

        DrawTextEx(font, text, position, (float) fontSize, (float) spacing, color);
    }
}

void windowStart() {
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 600;

    int x0 = 0, y0 = 0, nump, genp, aprox = 1000;
    float l = 5.0f, a = 5.0f;
    char buff1[256];
    double max, min;

    Point *p;
    p = rebuildGraph(&max, &min, &nump, aprox, l, a);

    double k = 0.5, ky = 0.5;
    double mx = (double) (screenWidth / (max - min)) * k;
    double my = (double) (screenWidth / (max - min)) * ky;


    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    InitWindow(screenWidth, screenHeight, "LAB 3, Plotter, Done by: Valentyn Valentiev");

    SetWindowMinSize(800, 600);
    SetTargetFPS(30);               // Set our game to run at 60 frames-per-second

    int display = GetCurrentMonitor();

    ScreenPoint *sp;
    sp = getGraphRelative(p, nump, &genp, screenWidth / 2 + x0, screenHeight / 2 + y0, mx, my, screenWidth, screenHeight);

    font = LoadFont("mecha.png");

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsWindowResized() && !IsWindowFullscreen()) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
            if (sp) {
                free(sp);
                sp = NULL;
            }
            mx = (double) (screenWidth / (max - min)) * k;
            my = (double) (screenWidth / (max - min)) * ky;
            sp = getGraphRelative(p, nump, &genp, screenWidth / 2 + x0, screenHeight / 2 + y0, mx, my, screenWidth, screenHeight);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (GetMouseY() < screenHeight - screenHeight / 10) {
                Vector2 delta = GetMouseDelta();
                x0 += (int) delta.x;
                y0 += (int) delta.y;
                if (sp) {
                    free(sp);
                    sp = NULL;
                }
                mx = (double) (screenWidth / (max - min)) * k;
                my = (double) (screenWidth / (max - min)) * ky;

                sp = getGraphRelative(p, nump, &genp, screenWidth / 2 + x0, screenHeight / 2 + y0, mx, my, screenWidth, screenHeight);
            } else {
                if (p) {
                    free(p);
                    p = NULL;
                }
                p = rebuildGraph(&max, &min, &nump, aprox, l, a);
                mx = (double) (screenWidth / (max - min)) * k;
                my = (double) (screenWidth / (max - min)) * ky;
                if (sp) {
                    free(sp);
                    sp = NULL;
                }
                sp = getGraphRelative(p, nump, &genp, screenWidth / 2 + x0, screenHeight / 2 + y0, mx, my, screenWidth, screenHeight);
            }
        }

        // check for alt + enter
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
            // see what display we are on right now
            if (IsWindowFullscreen()) {
                screenWidth = GetScreenWidth();
                screenHeight = GetScreenHeight();
                SetWindowSize(screenWidth, screenHeight);
            } else {
                screenWidth = GetMonitorWidth(display);
                screenHeight = GetMonitorHeight(display);
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
            }
            if (sp) {
                free(sp);
                sp = NULL;
            }
            mx = (double) (screenWidth / (max - min)) * k;
            my = (double) (screenWidth / (max - min)) * ky;
            sp = getGraphRelative(p, nump, &genp, screenWidth / 2 + x0, screenHeight / 2 + y0, mx, my, screenWidth, screenHeight);

            ToggleFullscreen();
        }

        float wheel = GetMouseWheelMove();
        if (wheel != 0 && IsKeyDown(KEY_X)) {
            register double tmp = k;

            k += (wheel * 0.05) * k;

            if (k < 0.01) k = 0.01;
            if (k > 10) k = 10;

            mx = (double) (screenWidth / (max - min)) * k;
            x0 += (int) (x0 * (k - tmp) / k);

            if (sp) {
                free(sp);
                sp = NULL;
            }
            sp = getGraphRelative(p, nump, &genp, screenWidth / 2 + x0, screenHeight / 2 + y0, mx, my, screenWidth, screenHeight);

        } else if (wheel != 0 && IsKeyDown(KEY_Y)) {
            register double tmp2 = ky;

            ky += (wheel * 0.05) * ky;

            if (ky < 0.01) ky = 0.01;
            if (ky > 10) ky = 10;

            my = (double) (screenWidth / (max - min)) * ky;
            y0 += (int) (y0 * (ky - tmp2) / ky);

            if (sp) {
                free(sp);
                sp = NULL;
            }
            sp = getGraphRelative(p, nump, &genp, screenWidth / 2 + x0, screenHeight / 2 + y0, mx, my, screenWidth, screenHeight);

        } else if (wheel != 0) {
            register double tmp = k;
            register double tmp2 = ky;

            k += (wheel * 0.05) * k;
            ky += (wheel * 0.05) * ky;

            if (k < 0.01) k = 0.01;
            if (ky < 0.01) ky = 0.01;

            if (k > 10) k = 10;
            if (ky > 10) ky = 10;

            x0 += (int) (x0 * (k - tmp) / k);
            y0 += (int) (y0 * (ky - tmp2) / ky);

            mx = (double) (screenWidth / (max - min)) * k;
            my = (double) (screenWidth / (max - min)) * ky;

            if (sp) {
                free(sp);
                sp = NULL;
            }
            sp = getGraphRelative(p, nump, &genp, screenWidth / 2 + x0, screenHeight / 2 + y0, mx, my, screenWidth, screenHeight);
        }

        const register int oX = screenWidth / 2 + x0;
        const register int oY = screenHeight / 2 + y0;
        const register int rK = ceil(2 / k);
        const register int rKy = ceil(2 / ky);


        const float ratx = (float) screenWidth / 800.0f, raty = (float) screenHeight / 600.0f;

        Rectangle slidernump;
        slidernump.x = 160;
        slidernump.y = screenHeight - screenHeight / 10.0f + 5 * raty;
        slidernump.height = 10 * raty;
        slidernump.width = 425 * ratx;

        Rectangle sliderL;
        sliderL.x = 60;
        sliderL.y = (screenHeight - screenHeight / 10.0f) + 20 * raty;
        sliderL.height = 10 * raty;
        sliderL.width = 300 * ratx;

        Rectangle sliderA;
        sliderA.x = 60;
        sliderA.y = (screenHeight - screenHeight / 10.0f) + 35 * raty;
        sliderA.height = 10 * raty;
        sliderA.width = 300 * ratx;


        BeginDrawing();

        ClearBackground((Color) {32, 34, 37, 255});

        if (GetMouseY() < screenHeight - screenHeight / 10) {
            SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
            Point testp;
            testp = findClosest(p, nump, ((GetMouseX() - x0 - screenWidth / 2.0) / k / (screenWidth / (max - min))), ((GetMouseY() - y0 - screenHeight / 2.0) / ky / (screenWidth / (max - min))));

            int x = oX + (int) round(testp.X * mx), y = oY + (int) round(testp.Y * my);

            DrawCircle(x, y, 5, BLUE);
            sprintf(buff1, "\t(%.3f, %.3f)", (float) testp.X, -(float) testp.Y);
            drawText(buff1, x, y, 20, BLUE);
        } else {
            SetMouseCursor(MOUSE_CURSOR_ARROW);
        }


        DrawLine(oX, 0, oX, screenHeight, (Color) {88, 101, 242, 255});
        DrawLine(0, oY, screenWidth, oY, (Color) {88, 101, 242, 255});

        DrawLine(oX, 0, oX - 5, 20, (Color) {87, 242, 135, 255});
        DrawLine(oX, 0, oX + 5, 20, (Color) {87, 242, 135, 255});

        DrawLine(screenWidth, oY, screenWidth - 20, oY - 5, (Color) {87, 242, 135, 255});
        DrawLine(screenWidth, oY, screenWidth - 20, oY + 5, (Color) {87, 242, 135, 255});

        for (register int i = 0; i < (screenWidth - x0) / k; i += rK * 100) {
            DrawLine(oX + (i * k), 0, oX + (i * k), screenHeight, (Color) {88, 101, 242, 80});
        }
        for (register int i = 0; i < (screenWidth - x0) / k; i += rK * 25) {
            DrawLine(oX + (i * k), 0, oX + (i * k), screenHeight, (Color) {88, 101, 242, 25});
        }
        for (register int i = 0; i < (screenWidth + x0) / k; i += rK * 100) {
            DrawLine(oX - (i * k), 0, oX - (i * k), screenHeight, (Color) {88, 101, 242, 80});
        }
        for (register int i = 0; i < (screenWidth + x0) / k; i += rK * 25) {
            DrawLine(oX - (i * k), 0, oX - (i * k), screenHeight, (Color) {88, 101, 242, 25});
        }

        for (register int i = 0; i < (screenHeight + y0) / ky; i += rKy * 100) {
            DrawLine(0, oY - (int) (i * ky), screenWidth, oY - (int) (i * ky), (Color) {88, 101, 242, 80});
        }
        for (register int i = 0; i < (screenHeight + y0) / ky; i += rKy * 25) {
            DrawLine(0, oY - (int) (i * ky), screenWidth, oY - (int) (i * ky), (Color) {88, 101, 242, 25});
        }
        for (register int i = 0; i < (screenHeight - y0) / ky - y0; i += rKy * 100) {
            DrawLine(0, oY + (int) (i * ky), screenWidth, oY + (int) (i * ky), (Color) {88, 101, 242, 80});
        }
        for (register int i = 0; i < (screenHeight - y0) / ky - y0; i += rKy * 25) {
            DrawLine(0, oY + (int) (i * ky), screenWidth, oY + (int) (i * ky), (Color) {88, 101, 242, 25});
        }

        for (register int i = 0; i < (screenWidth - x0) / k; i += rK * 100) {
            sprintf(buff1, "%.2lf", i / (screenWidth / (max - min)));
            DrawLine(oX + (i * k), oY + 10, oX + (i * k), oY - 10, (Color) {87, 242, 135, 255});
            drawText(buff1, oX + (i * k) - 30, oY + 20, 20, (Color) {87, 242, 135, 80});
        }

        for (register int i = rKy * 100; i < (screenHeight - y0) / ky; i += rKy * 100) {
            sprintf(buff1, "%.2lf", -i / (screenWidth / (max - min)));
            DrawLine(oX + 10, oY + (int) (i * ky), oX - 10, oY + (int) (i * ky), (Color) {87, 242, 135, 255});
            drawText(buff1, oX - 20, oY + (int) (i * ky) + 10, 20, (Color) {87, 242, 135, 80});
        }

        for (register int i = 0; i < (screenWidth + x0) / k; i += rK * 100) {
            sprintf(buff1, "%.2lf", -i / (screenWidth / (max - min)));
            DrawLine(oX - (i * k), oY + 10, oX - (i * k), oY - 10, (Color) {87, 242, 135, 255});
            drawText(buff1, oX - (i * k) - 30, oY + 20, 20, (Color) {87, 242, 135, 80});
        }

        for (register int i = rKy * 100; i < (screenHeight + y0) / ky; i += rKy * 100) {
            sprintf(buff1, "%.2lf", i / (screenWidth / (max - min)));
            DrawLine(oX + 10, oY - (int) (i * ky), oX - 10, oY - (int) (i * ky), (Color) {87, 242, 135, 255});
            drawText(buff1, oX - 20, oY - (int) (i * ky) + 10, 20, (Color) {87, 242, 135, 80});
        }
        for (register int i = 0; i < genp - 1; i++) {
            if (abs(sp[i + 1].Y - sp[i].Y) < screenHeight / 2)
                DrawLine(sp[i].X, sp[i].Y, sp[i + 1].X, sp[i + 1].Y, (Color) {87, 242, 135, 255});
        }
//b1B8iFfHRcg9e6WB
        DrawRectangle(5, screenHeight - screenHeight / 10, screenWidth - 10, screenHeight / 10 - 5, RAYWHITE);

        sprintf(buff1, "10000 : %d", genp);
        aprox = (int) GuiSlider(slidernump, "Number of points desired: 50", buff1, (float) aprox, 50, 10000);
        sprintf(buff1, "10.0 : %f", l);
        l = GuiSlider(sliderL, "l value: 0", buff1, l, 0.0f, 10.0f);
        sprintf(buff1, "10.0 : %f", a);
        a = GuiSlider(sliderA, "a value: 0", buff1, a, 0.0f, 10.0f);

        drawText("\tDone by:\n\t\tValentyn Valentiev", screenWidth - 150 * ratx, (int) ((screenHeight - screenHeight / 10.0) + 2 * raty), 20 * MIN(ratx, raty), RED);
        drawText("\nx = a + l * cos(t)\ny = a * tan(t) + l * sin(t)", 10, 0, 20, (Color) {87, 242, 135, 255});

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    if (sp) {
        free(sp);
        sp = NULL;
    }
    if (p) {
        free(p);
        p = NULL;
    }

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}


int main() {
    windowStart();
    return 0;
}