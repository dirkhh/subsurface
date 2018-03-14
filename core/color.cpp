// SPDX-License-Identifier: GPL-2.0
#include "color.h"

QMap<color_index_t, QVector<QColor> > profile_color;

void fill_profile_color()
{
#define COLOR(x, y, z) QVector<QColor>() << x << y << z;
	profile_color[SAC_1] = COLOR(FUNGREEN1, BLACK1_LOW_TRANS, FUNGREEN1);
	profile_color[SAC_2] = COLOR(APPLE1, BLACK1_LOW_TRANS, APPLE1);
	profile_color[SAC_3] = COLOR(ATLANTIS1, BLACK1_LOW_TRANS, ATLANTIS1);
	profile_color[SAC_4] = COLOR(ATLANTIS2, BLACK1_LOW_TRANS, ATLANTIS2);
	profile_color[SAC_5] = COLOR(EARLSGREEN1, BLACK1_LOW_TRANS, EARLSGREEN1);
	profile_color[SAC_6] = COLOR(HOKEYPOKEY1, BLACK1_LOW_TRANS, HOKEYPOKEY1);
	profile_color[SAC_7] = COLOR(TUSCANY1, BLACK1_LOW_TRANS, TUSCANY1);
	profile_color[SAC_8] = COLOR(CINNABAR1, BLACK1_LOW_TRANS, CINNABAR1);
	profile_color[SAC_9] = COLOR(REDORANGE1, BLACK1_LOW_TRANS, REDORANGE1);

	profile_color[VELO_STABLE] = COLOR(CAMARONE1, BLACK1_LOW_TRANS, CAMARONE1);
	profile_color[VELO_SLOW] = COLOR(LIMENADE1, BLACK1_LOW_TRANS, LIMENADE1);
	profile_color[VELO_MODERATE] = COLOR(RIOGRANDE1, BLACK1_LOW_TRANS, RIOGRANDE1);
	profile_color[VELO_FAST] = COLOR(PIRATEGOLD1, BLACK1_LOW_TRANS, PIRATEGOLD1);
	profile_color[VELO_CRAZY] = COLOR(RED1, BLACK1_LOW_TRANS, RED1);

	profile_color[PO2] = COLOR(APPLE1, BLACK1_LOW_TRANS, APPLE1);
	profile_color[PO2_ALERT] = COLOR(RED1, BLACK1_LOW_TRANS, RED1);
	profile_color[PN2] = COLOR(BLACK1_LOW_TRANS, BLACK1_LOW_TRANS, BLACK1_LOW_TRANS);
	profile_color[PN2_ALERT] = COLOR(RED1, BLACK1_LOW_TRANS, RED1);
	profile_color[PHE] = COLOR(PEANUT, BLACK1_LOW_TRANS, PEANUT);
	profile_color[PHE_ALERT] = COLOR(RED1, BLACK1_LOW_TRANS, RED1);
	profile_color[O2SETPOINT] = COLOR(RED1, BLACK1_LOW_TRANS, RED1);
	profile_color[CCRSENSOR1] = COLOR(TUNDORA1_MED_TRANS, BLACK1_LOW_TRANS, TUNDORA1_MED_TRANS);
	profile_color[CCRSENSOR2] = COLOR(ROYALBLUE2_LOW_TRANS, BLACK1_LOW_TRANS, ROYALBLUE2_LOW_TRANS);
	profile_color[CCRSENSOR3] = COLOR(PEANUT, BLACK1_LOW_TRANS, PEANUT);
	profile_color[SCR_OCPO2] = COLOR(PIRATEGOLD1_MED_TRANS, BLACK1_LOW_TRANS, PIRATEGOLD1_MED_TRANS);

	profile_color[PP_LINES] = COLOR(BLACK1_HIGH_TRANS, BLACK1_LOW_TRANS, BLACK1_HIGH_TRANS);

	profile_color[TEXT_BACKGROUND] = COLOR(CONCRETE1_LOWER_TRANS, WHITE1, CONCRETE1_LOWER_TRANS);
	profile_color[ALERT_BG] = COLOR(BROOM1_LOWER_TRANS, BLACK1_LOW_TRANS, BROOM1_LOWER_TRANS);
	profile_color[ALERT_FG] = COLOR(BLACK1_LOW_TRANS, WHITE1, BLACK1_LOW_TRANS);
	profile_color[EVENTS] = COLOR(REDORANGE1, BLACK1_LOW_TRANS, REDORANGE1);
	profile_color[SAMPLE_DEEP] = COLOR(QColor(Qt::red).darker(), BLACK1, PERSIANRED1);
	profile_color[SAMPLE_SHALLOW] = COLOR(QColor(Qt::red).lighter(), BLACK1_LOW_TRANS, PERSIANRED1);
	profile_color[SMOOTHED] = COLOR(REDORANGE1_HIGH_TRANS, BLACK1_LOW_TRANS, REDORANGE1_HIGH_TRANS);
	profile_color[MINUTE] = COLOR(MEDIUMREDVIOLET1_HIGHER_TRANS, BLACK1_LOW_TRANS, MEDIUMREDVIOLET1_HIGHER_TRANS);
	profile_color[TIME_GRID] = COLOR(WHITE1, BLACK1_HIGH_TRANS, TUNDORA1_MED_TRANS);
	profile_color[TIME_TEXT] = COLOR(FORESTGREEN1, BLACK1, FORESTGREEN1);
	profile_color[DEPTH_GRID] = COLOR(WHITE1, BLACK1_HIGH_TRANS, TUNDORA1_MED_TRANS);
	profile_color[MEAN_DEPTH] = COLOR(REDORANGE1_MED_TRANS, BLACK1_LOW_TRANS, REDORANGE1_MED_TRANS);
	profile_color[HR_PLOT] = COLOR(REDORANGE1_MED_TRANS, BLACK1_LOW_TRANS, REDORANGE1_MED_TRANS);
	profile_color[HR_TEXT] = COLOR(REDORANGE1_MED_TRANS, BLACK1_LOW_TRANS, REDORANGE1_MED_TRANS);
	profile_color[HR_AXIS] = COLOR(MED_GRAY_HIGH_TRANS, MED_GRAY_HIGH_TRANS, MED_GRAY_HIGH_TRANS);
	profile_color[DEPTH_BOTTOM] = COLOR(GOVERNORBAY1_MED_TRANS, BLACK1_HIGH_TRANS, GOVERNORBAY1_MED_TRANS);
	profile_color[DEPTH_TOP] = COLOR(MERCURY1_MED_TRANS, WHITE1_MED_TRANS, MERCURY1_MED_TRANS);
	profile_color[TEMP_TEXT] = COLOR(GOVERNORBAY2, BLACK1_LOW_TRANS, GOVERNORBAY2);
	profile_color[TEMP_PLOT] = COLOR(ROYALBLUE2_LOW_TRANS, BLACK1_LOW_TRANS, ROYALBLUE2_LOW_TRANS);
	profile_color[SAC_DEFAULT] = COLOR(WHITE1, BLACK1_LOW_TRANS, FORESTGREEN1);
	profile_color[BOUNDING_BOX] = COLOR(WHITE1, BLACK1_LOW_TRANS, TUNDORA1_MED_TRANS);
	profile_color[PRESSURE_TEXT] = COLOR(KILLARNEY1, BLACK1_LOW_TRANS, KILLARNEY1);
	profile_color[BACKGROUND] = COLOR(SPRINGWOOD1, WHITE1, SPRINGWOOD1);
	profile_color[BACKGROUND_TRANS] = COLOR(SPRINGWOOD1_MED_TRANS, WHITE1_MED_TRANS, SPRINGWOOD1_MED_TRANS);
	profile_color[CEILING_SHALLOW] = COLOR(REDORANGE1_HIGH_TRANS, BLACK1_HIGH_TRANS, REDORANGE1_HIGH_TRANS);
	profile_color[CEILING_DEEP] = COLOR(RED1_MED_TRANS, BLACK1_HIGH_TRANS, RED1_MED_TRANS);
	profile_color[CALC_CEILING_SHALLOW] = COLOR(FUNGREEN1_HIGH_TRANS, BLACK1_HIGH_TRANS, FUNGREEN1_HIGH_TRANS);
	profile_color[CALC_CEILING_DEEP] = COLOR(APPLE1_HIGH_TRANS, BLACK1_HIGH_TRANS, APPLE1_HIGH_TRANS);
	profile_color[TISSUE_PERCENTAGE] = COLOR(GOVERNORBAY2, BLACK1_LOW_TRANS, GOVERNORBAY2);
	profile_color[GF_LINE] = COLOR(BLACK1, BLACK1_LOW_TRANS, BLACK1);
	profile_color[AMB_PRESSURE_LINE] = COLOR(TUNDORA1_MED_TRANS, BLACK1_LOW_TRANS, ATLANTIS1);
#undef COLOR
}

QColor getColor(const color_index_t i, bool isGrayscale)
{
	if (profile_color.count() > i && i >= 0)
		return profile_color[i].at((isGrayscale) ? 1 : 0);
	return QColor(Qt::black);
}

QColor getSacColor(int sac, int avg_sac)
{
	int sac_index = 0;
	int delta = sac - avg_sac + 7000;

	sac_index = delta / 2000;
	if (sac_index < 0)
		sac_index = 0;
	if (sac_index > SAC_COLORS - 1)
		sac_index = SAC_COLORS - 1;
	return getColor((color_index_t)(SAC_COLORS_START_IDX + sac_index), false);
}

QColor getPressureColor(double density)
{
	QColor color;

	int h = ((int) (180.0 - 180.0 * density / 8.0));
	while (h < 0)
		h += 360;
	color.setHsv(h , 255, 255);
	return color;
}
