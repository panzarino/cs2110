/**
* @file graphics.c
* @author Zachary Panzarino
* @date 11/04/18
* @brief A graphics library for drawing geometry, for Homework 9 of Georgia Tech
*        CS 2110, Fall 2018.
*/

// Please take a look at the header file below to understand what's required for
// each of the functions.
#include "graphics.h"

// Don't touch this. It's used for sorting for the drawFilledPolygon function.
int int_cmp(const void *a, const void *b)
{
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;
    return *ia  - *ib;
}

Pixel noFilter(Pixel c) {
    // Don't touch this.
    return c;
}

// TODO: Complete according to the prototype in graphics.h
Pixel greyscaleFilter(Pixel c) {
    int r = (c >> 0) & ~(0xFFF << 5);
    int g = (c >> 5) & ~(0xFFF << 5);
    int b = (c >> 10) & ~(0xFFF << 5);
    int n = ((r * 77) + (g * 151) + (b * 28)) >> 8;
    c = (c & (~(0xFFF << 0))) | (n << 0);
    c = (c & (~(0xFFF << 5))) | (n << 5);
    c = (c & (~(0xFFF << 10))) | (n << 10);
    return c;
}

// TODO: Complete according to the prototype in graphics.h
Pixel redOnlyFilter(Pixel c) {
    c = (c & (~(0xFFF << 5))) | (0 << 5);
    c = (c & (~(0xFFF << 10))) | (0 << 10);
    return c;
}

// TODO: Complete according to the prototype in graphics.h
Pixel brighterFilter(Pixel c) {
    int r = (c >> 0) & ~(0xFFF << 5);
    int g = (c >> 5) & ~(0xFFF << 5);
    int b = (c >> 10) & ~(0xFFF << 5);
    int newR = r + ((0x1F - r) >> 1);
    int newG = g + ((0x1F - g) >> 1);
    int newB = b + ((0x1F - b) >> 1);
    c = (c & (~(0xFFF << 0))) | (newR << 0);
    c = (c & (~(0xFFF << 5))) | (newG << 5);
    c = (c & (~(0xFFF << 10))) | (newB << 10);
    return c;
}


// TODO: Complete according to the prototype in graphics.h
void drawPixel(Screen *screen, Vector coordinates, Pixel pixel) {
    if (coordinates.x < 0 || coordinates.x >= screen->size.x || coordinates.y < 0 || coordinates.y >= screen->size.y) {
        return;
    }
    screen->buffer[coordinates.y * screen->size.x + coordinates.x] = pixel;
}

// TODO: Complete according to the prototype in graphics.h
void drawFilledRectangle(Screen *screen, Rectangle *rectangle) {
    for (int r = 0; r < rectangle->size.y; r++) {
        for (int c = 0; c < rectangle->size.x; c++) {
            Vector *coordinates = malloc(sizeof(Vector));
            coordinates->x = rectangle->top_left.x + c;
            coordinates->y = rectangle->top_left.y + r;
            drawPixel(screen, *coordinates, rectangle->color);
            free(coordinates);
        }
    }
}

int signum(int x) {
    if (x < 0) {
        return -1;
    }
    if (x == 0) {
        return 0;
    }
    return 1;
}

// TODO: Complete according to the prototype in graphics.h
void drawLine(Screen *screen, Line *line) {
    int changed = 0;
    int x1 = line->start.x;
    int y1 = line->start.y;
    int x2 = line->end.x;
    int y2 = line->end.y;
    int x = x1;
    int y = y1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int signx = signum(x2 - x1);
    int signy = signum(y2 - y1);
    if (dy > dx) {
        int temp = dx;
        dx = dy;
        dy = temp;
        changed = 1;
    }
    int e = 2 * dy - dx;
    for (int i = 1; i <= dx; i++) {
        Vector *coordinates = malloc(sizeof(Vector));
        coordinates->x = x;
        coordinates->y = y;
        drawPixel(screen, *coordinates, line->color);
        free(coordinates);
        while (e >= 0) {
            if (changed) {
                x += signx;
            } else {
                y += signy;
            }
            e = e - 2 * dx;
        }
        if (changed) {
            y += signy;
        } else {
            x += signx;
        }
        e = e + 2 * dy;
    }
    Vector *coordinates = malloc(sizeof(Vector));
    coordinates->x = x2;
    coordinates->y = y2;
    drawPixel(screen, *coordinates, line->color);
    free(coordinates);
}

// TODO: Complete according to the prototype in graphics.h
void drawPolygon(Screen *screen, Polygon *polygon) {
    for (int i = 0; i < polygon->num_vertices; i++) {
        int j = i + 1;
        if (j >= polygon->num_vertices) {
            j = 0;
        }
        Line *line = malloc(sizeof(Line));
        line->start.x = polygon->vertices[i].x;
        line->start.y = polygon->vertices[i].y;
        line->end.x = polygon->vertices[j].x;
        line->end.y = polygon->vertices[j].y;
        line->color = polygon->color;
        drawLine(screen, line);
        free(line);
    }
}

// TODO: Complete according to the prototype in graphics.h
void drawFilledPolygon(Screen *screen, Polygon *polygon) {
    UNUSED(screen);
	int min_y = INT_MAX;
    int max_y = INT_MIN;

    // -------------------------------------------------------------------------
    // TODO: Here, write some code that will find the minimum and maximum
    // Y values that belong to vertices of the polygon, and store them as
    // min_y and max_y.
    // -------------------------------------------------------------------------

    for (int i = 0; i < polygon->num_vertices; i++) {
        if (polygon->vertices[i].y < min_y) {
            min_y = polygon->vertices[i].y;
        }
        if (polygon->vertices[i].y > max_y) {
            max_y = polygon->vertices[i].y;
        }
    }

    // -------------------------------------------------------------------------
    // END OF TODO
    // -------------------------------------------------------------------------

    // Now we iterate through the rows of our polygon
	for (int row = min_y; row <= max_y; row++) {
        // This variable contains the number of nodes found. We start with 0.
		int nNodes = 0;

        // This array will contain the X coords of the nodes we find.
        // Note that there are at most num_vertices of those. We allocate
        // that much room, but at any time only the first nNodes ints will
        // be our actual data.
        int nodeX[polygon->num_vertices];

        // This loop finds the nodes on this row. It intersects the line
        // segments between consecutive pairs of vertices with the horizontal
        // line corresponding to the row we're on. Don't worry about the
        // details, it just works.
		int j = polygon->num_vertices - 1;
		for (int i = 0; i < polygon->num_vertices; i++) {
			if ((polygon->vertices[i].y < row && polygon->vertices[j].y >= row) ||
				(polygon->vertices[j].y < row && polygon->vertices[i].y >= row)) {
				nodeX[nNodes++] = (polygon->vertices[i].x +
                    (row - polygon->vertices[i].y) *
                    (polygon->vertices[j].x - polygon->vertices[i].x) /
                    (polygon->vertices[j].y - polygon->vertices[i].y));
			}
			j = i;
		}

        // ---------------------------------------------------------------------
        // TODO: Make a call to qsort here to sort the nodeX array we made,
        // from small to large x coordinate. Note that there are nNodes elements
        // that we want to sort, and each is an integer. We give you int_cmp
        // at the top of the file to use as the comparator for the qsort call,
        // so you can just pass it to qsort and not need to write your own
        // comparator.
        // ---------------------------------------------------------------------

        qsort(nodeX, nNodes, sizeof(int), int_cmp);

        // ---------------------------------------------------------------------
        // END OF TODO
        // ---------------------------------------------------------------------



        // ---------------------------------------------------------------------
        // TODO: Now we fill the x coordinates corresponding to the interior of
        // the polygon. Note that after every node we switch sides on the
        // polygon, that is, if we are on the outside, when we pass a node we
        // end up on the inside, and if are inside, we end up on the outside.
        // As a result, all you need to do is start at the 0th node, iterate
        // through all of the even-indexed nodes, and fill until the next node.
        // For example, you need to fill between nodes 0-1, between nodes 2-3,
        // nodes 4-5 etc. but not between nodes 1-2, or nodes 3-4.
        // ---------------------------------------------------------------------

        // ran out of time oops

        // ---------------------------------------------------------------------
        // END OF TODO
        // ---------------------------------------------------------------------
	}
}

// TODO: Complete according to the prototype in graphics.h
void drawRectangle(Screen *screen, Rectangle *rectangle) {
    Vector verts[4];
    verts[0] = (Vector){rectangle->top_left.x, rectangle->top_left.y};
    verts[1] = (Vector){rectangle->top_left.x + rectangle->size.x - 1, rectangle->top_left.y};
    verts[2] = (Vector){rectangle->top_left.x + rectangle->size.x - 1, rectangle->top_left.y + rectangle->size.y - 1};
    verts[3] = (Vector){rectangle->top_left.x, rectangle->top_left.y + rectangle->size.y - 1};
    Polygon polygon = {verts, 4, rectangle->color};
    drawPolygon(screen, &polygon);
}

// TODO: Complete according to the prototype in graphics.h
void drawCircle(Screen *screen, Circle *circle) {
    int mx = circle->center.x;
    int my = circle->center.y;
    int y = circle->radius;
    int x = 0;
    int d = 1 - y;
    while(x <= y) {
        Vector *coordinates = malloc(sizeof(Vector));
        coordinates->x = mx + x;
        coordinates->y = my + y;
        drawPixel(screen, *coordinates, circle->color);
        coordinates->x = mx + x;
        coordinates->y = my - y;
        drawPixel(screen, *coordinates, circle->color);
        coordinates->x = mx - x;
        coordinates->y = my + y;
        drawPixel(screen, *coordinates, circle->color);
        coordinates->x = mx - x;
        coordinates->y = my - y;
        drawPixel(screen, *coordinates, circle->color);
        coordinates->x = mx + y;
        coordinates->y = my + x;
        drawPixel(screen, *coordinates, circle->color);
        coordinates->x = mx + y;
        coordinates->y = my - x;
        drawPixel(screen, *coordinates, circle->color);
        coordinates->x = mx - y;
        coordinates->y = my + x;
        drawPixel(screen, *coordinates, circle->color);
        coordinates->x = mx - y;
        coordinates->y = my - x;
        drawPixel(screen, *coordinates, circle->color);
        free(coordinates);
        if (d < 0) {
            d = d + 2 * x + 3;
            x += 1;
        } else {
            d = d + 2 * (x - y) + 5;
            x += 1;
            y -= 1;
        }
    }
}

// TODO: Complete according to the prototype in graphics.h
void drawFilledCircle(Screen *screen, Circle *circle) {
    int mx = circle->center.x;
    int my = circle->center.y;
    int y = circle->radius;
    int x = 0;
    int d = 1 - y;
    while(x <= y) {
        Vector *p1 = malloc(sizeof(Vector));
        p1->x = mx + x;
        p1->y = my + y;
        Vector *p2 = malloc(sizeof(Vector));
        p2->x = mx + x;
        p2->y = (my > y) ? (my - y) : 0;
        Line *line = malloc(sizeof(Line));
        line->color = circle->color;
        line->start = *p1;
        line->end = *p2;
        drawLine(screen, line);
        if (mx >= x) {
            p1->x = mx - x;
            p1->y = my + y;
            p2->x = mx - x;
            p2->y = (my > y) ? (my - y) : 0;
            line->start = *p1;
            line->end = *p2;
            drawLine(screen, line);
        }
        p1->x = mx + y;
        p1->y = my + x;
        p2->x = mx + y;
        p2->y = (my > x) ? (my - x) : 0;
        line->start = *p1;
        line->end = *p2;
        drawLine(screen, line);
        if (mx >= y) {
            p1->x = mx - y;
            p1->y = my + x;
            p2->x = mx - y;
            p2->y = (my > x) ? (my - x) : 0;
            line->start = *p1;
            line->end = *p2;
            drawLine(screen, line);
        }
        free(line);
        free(p1);
        free(p2);
        if (d < 0) {
            d = d + 2 * x + 3;
            x += 1;
        } else {
            d = d + 2 * (x - y) + 5;
            x += 1;
            y -= 1;
        }
    }
}

// TODO: Complete according to the prototype in graphics.h
void drawImage(Screen *screen, Image *image, Pixel (*colorFilter)(Pixel)) {
    int buff = 0;
    for (int r = 0; r < image->size.y; r++) {
        for (int c = 0; c < image->size.x; c++) {
            Vector *coordinates = malloc(sizeof(Vector));
            coordinates->x = image->top_left.x + c;
            coordinates->y = image->top_left.y + r;
            Pixel color = colorFilter(image->buffer[buff++]);
            drawPixel(screen, *coordinates, color);
            free(coordinates);
        }
    }
}

// TODO: Complete according to the prototype in graphics.
Image rotateImage(Image *image, int degrees) {
     UNUSED(image);
     UNUSED(degrees);

    // ran out of time oops

     // This is just here to keep the compiler from complaining.
     // Delete this line when you're starting to work on this function.
     return (Image){(Vector){0, 0}, (Vector){0, 0}, malloc(1)};
}
