#include "statscolors.h"
#include "statstranslations.h"

// Colors created using the Chroma.js Color Palette Helper
// https://vis4.net/palettes/#/50|d|00108c,3ed8ff,ffffe0|ffffe0,ff005e,743535|1|1
static const QColor binColors[] = {
	QRgb(0x00108c), QRgb(0x0f1c92), QRgb(0x1a2798), QRgb(0x23319d), QRgb(0x2a3ba3),
	QRgb(0x3144a8), QRgb(0x374eae), QRgb(0x3e58b3), QRgb(0x4461b8), QRgb(0x4b6bbd),
	QRgb(0x5274c2), QRgb(0x587ec7), QRgb(0x6088cc), QRgb(0x6791d0), QRgb(0x6f9bd4),
	QRgb(0x78a5d8), QRgb(0x81aedb), QRgb(0x8ab8df), QRgb(0x95c2e2), QRgb(0xa0cbe4),
	QRgb(0xacd5e6), QRgb(0xb9dee7), QRgb(0xc7e7e7), QRgb(0xd7efe7), QRgb(0xeaf8e4),
	QRgb(0xfff5d8), QRgb(0xffead0), QRgb(0xffe0c8), QRgb(0xffd5c0), QRgb(0xffcab8),
	QRgb(0xffbfb0), QRgb(0xffb4a8), QRgb(0xffa99f), QRgb(0xfc9e98), QRgb(0xf99490),
	QRgb(0xf48b89), QRgb(0xf08182), QRgb(0xea787b), QRgb(0xe46f74), QRgb(0xde666e),
	QRgb(0xd75e67), QRgb(0xcf5661), QRgb(0xc64f5b), QRgb(0xbd4855), QRgb(0xb3434f),
	QRgb(0xa83e49), QRgb(0x9d3a44), QRgb(0x90383f), QRgb(0x83363a), QRgb(0x743535)
};

StatsTheme::StatsTheme() :
	scatterItemTexture(nullptr),
	scatterItemSelectedTexture(nullptr),
	scatterItemHighlightedTexture(nullptr),
	selectedTexture(nullptr)
{
}

class StatsThemeLight : public StatsTheme {
public:
	StatsThemeLight()
	{
		backgroundColor = Qt::white;
		fillColor = QColor(0x44, 0x76, 0xaa);
		borderColor = QColor(0x66, 0xb2, 0xff);
		selectedColor = QColor(0xaa, 0x76, 0x44);
		selectedBorderColor = QColor(0xff, 0xb2, 0x66);
		highlightedColor = Qt::yellow;
		highlightedBorderColor = QColor(0xaa, 0xaa, 0x22);
		darkLabelColor = Qt::black;
		lightLabelColor = Qt::white;
		axisColor = Qt::black;
		gridColor = QColor(0xcc, 0xcc, 0xcc);
		informationBorderColor = Qt::black;
		informationColor = QColor(0xff, 0xff, 0x00, 192); // Note: fourth argument is opacity
		legendColor = QColor(0x00, 0x8e, 0xcc, 192); // Note: fourth argument is opacity
		legendBorderColor = Qt::black;
		quartileMarkerColor = Qt::red;
		regressionItemColor = Qt::red;
		meanMarkerColor = Qt::green;
		medianMarkerColor = Qt::red;
		selectionLassoColor = Qt::black;
		selectionOverlayColor = Qt::lightGray;

		// use a light version of the application font for axis labels, axis title and chart title
		axisLabelFont.setWeight(QFont::Light);
		axisTitleFont.setWeight(QFont::Light);
		titleFont.setWeight(QFont::Light);
	}
private:
	QString name() const
	{
		return StatsTranslations::tr("Light");
	}

	// Pick roughly equidistant colors out of the color set above
	// if we need more bins than we have colors (what chart is THAT?) simply loop
	QColor binColor(int bin, int numBins) const override
	{
		if (numBins == 1 || bin < 0 || bin >= numBins)
			return fillColor;
		if (numBins > (int)std::size(binColors))
			return binColors[bin % std::size(binColors)];

		// use integer math to spread out the indices
		int idx = bin * (std::size(binColors) - 1) / (numBins - 1);
		return binColors[idx];
	}

	QColor labelColor(int bin, size_t numBins) const override
	{
		return (binColor(bin, numBins).lightness() < 150) ? lightLabelColor : darkLabelColor;
	}
};

static const QColor binColorsDark[] = {
	QRgb(0xffef73), QRgb(0xf0e36d), QRgb(0xe5d867), QRgb(0xdcce62), QRgb(0xd5c45c),
	QRgb(0xcebb57), QRgb(0xc8b151), QRgb(0xc1a74c), QRgb(0xbb9e47), QRgb(0xb49442),
	QRgb(0xad8b3d), QRgb(0xa78138), QRgb(0x9f7733), QRgb(0x986e2f), QRgb(0x90642b),
	QRgb(0x875a27), QRgb(0x7e5124), QRgb(0x754720), QRgb(0x6a3d1d), QRgb(0x5f341b),
	QRgb(0x532a19), QRgb(0x462118), QRgb(0x381818), QRgb(0x281018), QRgb(0x15071b),
	QRgb(0x000a27), QRgb(0x00152f), QRgb(0x001f37), QRgb(0x002a3f), QRgb(0x003547),
	QRgb(0x00404f), QRgb(0x004b57), QRgb(0x005660), QRgb(0x036167), QRgb(0x066b6f),
	QRgb(0x0b7476), QRgb(0x0f7e7d), QRgb(0x158784), QRgb(0x1b908b), QRgb(0x219991),
	QRgb(0x28a198), QRgb(0x30a99e), QRgb(0x39b0a4), QRgb(0x42b7aa), QRgb(0x4cbcb0),
	QRgb(0x57c1b6), QRgb(0x62c5bb), QRgb(0x6fc7c0), QRgb(0x7cc9c5), QRgb(0x8bcaca)
};

class StatsThemeDark : public StatsTheme {
public:
	StatsThemeDark()
	{
		backgroundColor = Qt::black;
		fillColor = QColor(0xbb, 0x89, 0x55);
		borderColor = QColor(0x99, 0x4d, 0x00);
		selectedColor = QColor(0x55, 0x89, 0xbb);
		selectedBorderColor = QColor(0x00, 0x4d, 0x99);
		highlightedColor = Qt::blue;
		highlightedBorderColor = QColor(0x55, 0x55, 0xdd);
		darkLabelColor = Qt::white;
		lightLabelColor = Qt::black;
		axisColor = Qt::white;
		gridColor = QColor(0x33, 0x33, 0x33);
		informationBorderColor = Qt::white;
		informationColor = QColor(0x00, 0x00, 0xff, 192); // Note: fourth argument is opacity
		legendColor = QColor(0xff, 0x71, 0x33, 192); // Note: fourth argument is opacity
		legendBorderColor = Qt::white;
		quartileMarkerColor = Qt::cyan;
		regressionItemColor = Qt::cyan;
		meanMarkerColor = Qt::magenta;
		medianMarkerColor = Qt::cyan;
		selectionLassoColor = Qt::white;
		selectionOverlayColor = Qt::darkGray;

		// use a bold version of the application font for axis labels, axis title and chart title
		axisLabelFont.setWeight(QFont::Bold);
		axisTitleFont.setWeight(QFont::Bold);
		titleFont.setWeight(QFont::Bold);
	}
private:
	QString name() const
	{
		return StatsTranslations::tr("Dark");
	}

	// Pick roughly equidistant colors out of the color set above
	// if we need more bins than we have colors (what chart is THAT?) simply loop
	QColor binColor(int bin, int numBins) const override
	{
		if (numBins == 1 || bin < 0 || bin >= numBins)
			return fillColor;
		if (numBins > (int)std::size(binColorsDark))
			return binColors[bin % std::size(binColorsDark)];

		// use integer math to spread out the indices
		int idx = bin * (std::size(binColors) - 1) / (numBins - 1);
		return binColors[idx];
	}

	QColor labelColor(int bin, size_t numBins) const override
	{
		return (binColor(bin, numBins).lightness() < 150) ? darkLabelColor : lightLabelColor;
	}
};

// The vector of themes is generated on first use. Thus, the constructors are run
// once the overall application is initialized. This ensures that the themes'
// constructors can access the settings, etc.
static std::vector<std::unique_ptr<const StatsTheme>> statsThemes;
const std::vector<std::unique_ptr<const StatsTheme>> &getStatsThemes()
{
	if (statsThemes.empty()) {
		statsThemes.reserve(2);
		statsThemes.emplace_back(new StatsThemeLight);
		statsThemes.emplace_back(new StatsThemeDark);
	}
	return statsThemes;
}
