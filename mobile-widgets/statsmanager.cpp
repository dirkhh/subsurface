// SPDX-License-Identifier: GPL-2.0
#include "statsmanager.h"

StatsManager::StatsManager() : view(nullptr)
{
	// Test: show some random data. Let's see what happens.
	state.var1Changed(2);
	state.var2Changed(3);
	state.binner2Changed(2);
	updateUi();
}

StatsManager::~StatsManager()
{
}

void StatsManager::init(StatsView *v, QObject *o)
{
	if (!v)
		fprintf(stderr, "StatsManager::init(): no StatsView - statistics will not work.\n");
	view = v;
}

void StatsManager::doit()
{
	if (!view)
		return;
	view->plot(state);
}

static void setVariableList(const StatsState::VariableList &list, QStringList &stringList, int &idx)
{
	stringList.clear();
	for (const StatsState::Variable &v: list.variables)
		stringList.push_back(v.name);
	idx = list.selected;
}

static void setBinnerList(const StatsState::BinnerList &list, QStringList &stringList, int &idx)
{
	stringList.clear();
	for (const QString &v: list.binners)
		stringList.push_back(v);
	idx = list.selected;
}

void StatsManager::updateUi()
{
	uiState = state.getUIState();
	setVariableList(uiState.var1, var1List, var1Index);
	setBinnerList(uiState.binners1, binner1List, binner1Index);
	setVariableList(uiState.var2, var2List, var2Index);
	setBinnerList(uiState.binners2, binner2List, binner2Index);
	setVariableList(uiState.operations2, operation2List, operation2Index);
	chartList = state.usableCharts();
	var1ListChanged();
	binner1ListChanged();
	var2ListChanged();
	binner2ListChanged();
	operation2ListChanged();
	var1IndexChanged();
	binner1IndexChanged();
	var2IndexChanged();
	binner2IndexChanged();
	operation2IndexChanged();
	chartListChanged();

	if (view)
		view->plot(state);
}

void StatsManager::var1Changed(int idx)
{
	if (uiState.var1.variables.empty())
		return;
	idx = std::clamp(idx, 0, (int)uiState.var1.variables.size());
	state.var1Changed(uiState.var1.variables[idx].id);
	updateUi();
}

void StatsManager::var1BinnerChanged(int idx)
{
	state.binner1Changed(idx);
	updateUi();
}

void StatsManager::var2Changed(int idx)
{
	if (uiState.var2.variables.empty())
		return;
	idx = std::clamp(idx, 0, (int)uiState.var2.variables.size());
	state.var2Changed(uiState.var2.variables[idx].id);
	updateUi();
}

void StatsManager::var2BinnerChanged(int idx)
{
	state.binner2Changed(idx);
	updateUi();
}

void StatsManager::var2OperationChanged(int idx)
{
	if (uiState.operations2.variables.empty())
		return;
	idx = std::clamp(idx, 0, (int)uiState.operations2.variables.size());
	state.var2OperationChanged(uiState.operations2.variables[idx].id);
	updateUi();
}

void StatsManager::setChart(int id)
{
	state.chartChanged(id);
	updateUi();
}
