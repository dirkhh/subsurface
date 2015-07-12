#ifndef PRINTOPTIONS_H
#define PRINTOPTIONS_H

#include <QWidget>

#include "ui_printoptions.h"

struct print_options {
	enum print_type {
		DIVELIST,
		TABLE,
		STATISTICS
	} type;
	enum print_template {
		ONE_DIVE,
		TWO_DIVE,
		CUSTOM
	} p_template;
	bool print_selected;
	bool color_selected;
	bool landscape;
};

struct template_options {
	int font_index;
	int color_palette_index;
	double font_size;
	double line_spacing;
	struct color_palette_struct {
		QColor color1;
		QColor color2;
		QColor color3;
		QColor color4;
		QColor color5;
		bool operator!=(const color_palette_struct &other) const {
			return other.color1 != color1
					|| other.color2 != color2
					|| other.color3 != color3
					|| other.color4 != color4
					|| other.color5 != color5;
		}
	} color_palette;
	bool operator!=(const template_options &other) const {
		return other.font_index != font_index
				|| other.color_palette_index != color_palette_index
				|| other.font_size != font_size
				|| other.line_spacing != line_spacing
				|| other.color_palette != color_palette;
	}
 };

extern template_options::color_palette_struct almond_colors, custom_colors;

// should be based on a custom QPrintDialog class
class PrintOptions : public QWidget {
	Q_OBJECT

public:
	explicit PrintOptions(QWidget *parent, struct print_options *printOpt, struct template_options *templateOpt);
	void setup();

private:
	Ui::PrintOptions ui;
	struct print_options *printOptions;
	struct template_options *templateOptions;
	bool hasSetupSlots;

private
slots:
	void printInColorClicked(bool check);
	void printSelectedClicked(bool check);
	void on_radioStatisticsPrint_clicked(bool check);
	void on_radioTablePrint_clicked(bool check);
	void on_radioDiveListPrint_clicked(bool check);
	void on_printTemplate_currentIndexChanged(int index);
	void on_editButton_clicked();
};

#endif // PRINTOPTIONS_H
