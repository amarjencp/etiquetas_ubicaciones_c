#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <zint.h>
#include <hpdf.h>

#define A6_WIDTH (148.0 * 2.83464567)  // 148 mm in points
#define A6_HEIGHT (105.0 * 2.83464567) // 105 mm in points

// Function to generate barcode and save as PNG
void generate_barcode(const char *code, const char *filename) {
    struct zint_symbol *my_symbol = ZBarcode_Create();
    strcpy(my_symbol->outfile, filename);
    my_symbol->symbology = BARCODE_CODE128;
    my_symbol->output_options = BOLD_TEXT;
    my_symbol->height = 40;
    my_symbol->scale = 1;
    ZBarcode_Encode_and_Print(my_symbol, (unsigned char *)code, 0, 0);
    ZBarcode_Delete(my_symbol);
}

// Function to create a label and add it to a PDF page
void create_label(HPDF_Doc pdf, HPDF_Font font, int zona, int bloque, int fila, int altura, int is_single_barcode) {
    char code1[32], code2[32];
    snprintf(code1, sizeof(code1), "%02d%02d%02d%02d", zona, bloque, fila, altura);
    snprintf(code2, sizeof(code2), "%02d%02d%02d%02d", zona, bloque, fila, altura + 1);

    char filename1[64], filename2[64];
    snprintf(filename1, sizeof(filename1), "barcode_%s.png", code1);
    snprintf(filename2, sizeof(filename2), "barcode_%s.png", code2);

    generate_barcode(code1, filename1);
    if (!is_single_barcode) {
        generate_barcode(code2, filename2);
    }

    // Create a new page for each label
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetWidth(page, A6_WIDTH);
    HPDF_Page_SetHeight(page, A6_HEIGHT);
    HPDF_Page_SetRotate(page, 90);  // Set landscape orientation

    // Load barcode images into PDF
    HPDF_Image image1 = HPDF_LoadPngImageFromFile(pdf, filename1);
    HPDF_Image image2 = is_single_barcode ? NULL : HPDF_LoadPngImageFromFile(pdf, filename2);

    // Calculate positions and sizes
    HPDF_REAL top_row_height = A6_HEIGHT * 0.3;
    HPDF_REAL bottom_row_height = A6_HEIGHT - top_row_height;
    HPDF_REAL bottom_col_width = A6_WIDTH / 2;

    // Set font and draw text in top row
    HPDF_Page_SetFontAndSize(page, font, 50);
    HPDF_Page_BeginText(page);

    char label[32];
    snprintf(label, sizeof(label), "%02d - %02d - %02d", zona, bloque, fila);
    HPDF_REAL text_width = HPDF_Page_TextWidth(page, label);
    HPDF_Page_TextOut(page, (A6_WIDTH - text_width) / 2, A6_HEIGHT - 60, label);
    HPDF_Page_EndText(page);

    // Rotate and draw barcodes in bottom row
    HPDF_Page_GSave(page);
    HPDF_Page_Concat(page, 0, 1, -1, 0, 160, 50);  // Rotate image 90 degrees clockwise
    HPDF_Page_DrawImage(page, image1, 0, 0, bottom_row_height - 80, bottom_col_width - 120 );
    HPDF_Page_GRestore(page);

    if (!is_single_barcode) {
        HPDF_Page_GSave(page);
        HPDF_Page_Concat(page, 0, 1, -1, 0, 160 + bottom_col_width, 50);  // Rotate image 90 degrees clockwise
        HPDF_Page_DrawImage(page, image2, 0, 0, bottom_row_height - 80, bottom_col_width - 120);
        HPDF_Page_GRestore(page);
    }

    // Draw text labels for the heights
    HPDF_Page_BeginText(page);
    snprintf(label, sizeof(label), "%1d", altura);
    HPDF_Page_TextOut(page, 10, 20, label);

    if (!is_single_barcode) {
        snprintf(label, sizeof(label), "%1d", altura + 1);
        HPDF_Page_TextOut(page, bottom_col_width + 10, 20, label);
    }
    HPDF_Page_EndText(page);

    // Draw lines to separate rows and columns
    HPDF_Page_SetLineWidth(page, 5.0);
    HPDF_Page_MoveTo(page, 0, A6_HEIGHT - top_row_height);
    HPDF_Page_LineTo(page, A6_WIDTH, A6_HEIGHT - top_row_height);
    HPDF_Page_Stroke(page);

    HPDF_Page_MoveTo(page, bottom_col_width, 0);
    HPDF_Page_LineTo(page, bottom_col_width, A6_HEIGHT - top_row_height);
    HPDF_Page_Stroke(page);

    // Remove temporary barcode files
    remove(filename1);
    if (!is_single_barcode) {
        remove(filename2);
    }
}

void create_labels_for_bloque(HPDF_Doc pdf, HPDF_Font font, int zona, int bloque, int filas, int alturas) {
    for (int fila = 1; fila <= filas; fila++) {
        for (int altura = 1; altura <= alturas; altura += 2) {
            int is_single_barcode = (altura + 1 > alturas);
            create_label(pdf, font, zona, bloque, fila, altura, is_single_barcode);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <zona> <bloques> <filas> <alturas>\n", argv[0]);
        return 1;
    }

    int zona = atoi(argv[1]);
    int bloques = atoi(argv[2]);
    int filas = atoi(argv[3]);
    int alturas = atoi(argv[4]);

    char filename[64];

    for (int bloque = 1; bloque <= bloques; bloque++) {
        snprintf(filename, sizeof(filename), "./output/labels_zona%02d_bloque%02d.pdf", zona, bloque);

        HPDF_Doc pdf = HPDF_New(NULL, NULL);
        HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

        const char *font_path = "./libertine/LinLibertine_RB.ttf";
        const char *font_name = HPDF_LoadTTFontFromFile(pdf, font_path, HPDF_TRUE);
        HPDF_Font font = HPDF_GetFont(pdf, font_name, NULL);

        create_labels_for_bloque(pdf, font, zona, bloque, filas, alturas);

        HPDF_SaveToFile(pdf, filename);
        HPDF_Free(pdf);
    }

    return 0;
}
