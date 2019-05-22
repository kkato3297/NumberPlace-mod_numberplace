#include "NumberPlace.hpp"
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <string>
#ifdef _WINDOWS
#include <Windows.h>
#endif

#define VERSION 3

class StopWatch
{
private:
#ifdef _WINDOWS
	typedef __int64 Time;
	const Time _CLOCKS_PER_SEC = ([]() {
		LARGE_INTEGER period;
		QueryPerformanceFrequency(&period);

		return period.QuadPart;
	})();
#else
	typedef clock_t Time;
	const Time _CLOCKS_PER_SEC = CLOCKS_PER_SEC;
#endif
	Time m_startTime;
	Time m_stopTime;

	Time GetTimeNow(void) {
#ifdef _WINDOWS
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		return time.QuadPart;
#else
		return clock();
#endif
	};

public:
	void Start(void) {
		m_startTime = GetTimeNow();
	};

	void Stop(void) {
		m_stopTime = GetTimeNow();
	};

	double GetTime(void) {
		return static_cast<double>(m_stopTime - m_startTime) / _CLOCKS_PER_SEC;
	};
};

template<class T>
vector<T> Sort(vector<T> p_list)
{
	vector<T> l_result(p_list);

	sort(l_result.begin(), l_result.end());

	return l_result;
}

template<class T>
vector<T> Unique(vector<T> p_list)
{
	vector<T> l_result(p_list);

	l_result.erase(unique(l_result.begin(), l_result.end()), l_result.end());

	return l_result;
}

vector<size_t> Range(size_t p_first = 1, size_t p_end = static_cast<size_t>(-1), size_t p_step = 1)
{
	const size_t	l_length = (p_end - p_first + 1) / p_step;
	size_t			l_counter = p_first;
	vector<size_t>	l_result = vector<size_t>(l_length);

	generate_n(l_result.begin(), l_length, [&]() {
		const size_t l_value = l_counter;
		l_counter += p_step;
		return l_value;
	});

	return l_result;
}

#ifdef _WINDOWS
LARGE_INTEGER operator-(LARGE_INTEGER &p_a, LARGE_INTEGER &p_b)
{
	LARGE_INTEGER l_result;

	l_result.QuadPart = p_a.QuadPart - p_b.QuadPart;

	return l_result;
}
#endif

NumberPlace::NumberPlace(size_t p_width, size_t p_height)
	: m_cols(p_width), m_rows(p_height),
	  m_rowList(vector<vector<size_t>>(m_rows)),
	  m_colList(vector<vector<size_t>>(m_cols)),
	  m_childList(vector<vector<size_t>>(static_cast<size_t>(sqrt(m_rows)) * static_cast<size_t>(sqrt(m_cols)))),
	  m_cellList(vector<size_t>(m_rows * m_cols)),
	  m_cellListAnswer(vector<size_t>(m_rows * m_cols))
{}

void NumberPlace::Create(void)
{
	StopWatch l_stopWatch;

	l_stopWatch.Start();

#if VERSION == 1
	srand(static_cast<unsigned int>(time(nullptr)));

	for (auto i = 0; i < m_cellList.size(); i++) {
		int l_rowIndex = i / 9, l_colIndex = i % 9;
		auto &l_usedRowList = m_rowList[l_rowIndex], &l_usedColList = m_colList[l_colIndex];
		auto &l_usedChildList = m_childList[3 * (l_rowIndex / 3) + l_colIndex / 3];

		auto m_unusedList = ([
			&l_rowIndex, &l_colIndex,
				&l_usedRowList, &l_usedColList, &l_usedChildList
		]() {
			vector<int> l_result;
			vector<int> l_temp;

			l_temp.insert(l_temp.end(), l_usedRowList.cbegin(), l_usedRowList.cend());
			l_temp.insert(l_temp.end(), l_usedColList.cbegin(), l_usedColList.cend());
			l_temp.insert(l_temp.end(), l_usedChildList.cbegin(), l_usedChildList.cend());

			std::sort(l_temp.begin(), l_temp.end());
			l_temp.erase(std::unique(l_temp.begin(), l_temp.end()), l_temp.end());

			for (auto j : { 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {
				const auto l_index = std::find(l_temp.cbegin(), l_temp.cend(), j);
				if (l_index == l_temp.end()) {
					l_result.push_back(j);
				}
			}

			return l_result;
		})();

		std::wcout << L"point: (" << l_colIndex << L", " << l_rowIndex << L")" << std::endl;
		std::wcout << L"\tlist: [";
		for (int i = 0; i < m_unusedList.size(); i++) {
			std::wcout << m_unusedList[i] << ((i != m_unusedList.size() - 1) ? L", " : L"");
		}
		std::wcout << L"]" << std::endl;
		int l_answer = m_unusedList[static_cast<int>(rand() / (RAND_MAX + 1.0) * m_unusedList.size())];

		l_usedRowList.push_back(l_answer);
		l_usedColList.push_back(l_answer);
		l_usedChildList.push_back(l_answer);

		{
			array<int, 81> l_list = GetCellList();

			for (int i = 0; i < 9; i++) {
				std::wcout << L"+---+---+---+---+---+---+---+---+---+" << std::endl;
				for (int j = 0; j < 9; j++) {
					std::wcout << L"+ " << l_list[i * 9 + j] << L" ";
				}
				std::wcout << L"+" << std::endl;
			}
			std::wcout << L"+---+---+---+---+---+---+---+---+---+" << std::endl;
		}

		m_cellList[i] = l_answer;
	}
#elif VERSION == 2
	do {
		m_rowList = vector<vector<size_t>>(m_rows);
		m_colList = vector<vector<size_t>>(m_cols);
		m_childList = vector<vector<size_t>>(static_cast<size_t>(sqrt(m_rows)) * static_cast<size_t>(sqrt(m_cols)));
		m_cellList = vector<size_t>(m_rows * m_cols);

		for (size_t i = 0; i < m_rows; i++) {
			vector<size_t> l_result = vector<size_t>(9);
			vector<vector<size_t>> l_usedRowList(m_rowList.size()), l_usedColList(m_colList.size()), l_usedChildList(m_childList.size());
			bool l_isResult;

			do {
				l_isResult = false;

				([&]() {
					l_usedRowList = vector<vector<size_t>>(m_rowList);
					l_usedColList = vector<vector<size_t>>(m_colList);
					l_usedChildList = vector<vector<size_t>>(m_childList);
				})();

				([&]() {
					for (size_t j = 0; j < m_cols; j++) {
						auto m_unusedList = ([&]() {
							vector<size_t> l_temp, l_candidate = Range(1, 9);

							l_temp.insert(l_temp.end(), l_usedRowList[i].cbegin(), l_usedRowList[i].cend());
							l_temp.insert(l_temp.end(), l_usedColList[j].cbegin(), l_usedColList[j].cend());
							l_temp.insert(l_temp.end(), l_usedChildList[3 * (i / 3) + j / 3].cbegin(), l_usedChildList[3 * (i / 3) + j / 3].cend());

							std::sort(l_temp.begin(), l_temp.end());
							l_temp.erase(std::unique(l_temp.begin(), l_temp.end()), l_temp.end());
							for (auto value : l_temp) {
								l_candidate.erase(remove(l_candidate.begin(), l_candidate.end(), value), l_candidate.end());
							}

							return l_candidate;
						})();

#if _DEBUG
						std::wcout << L"\tlist: [";
						for (size_t i = 0; i < m_unusedList.size(); i++) {
							std::wcout << m_unusedList[i] << ((i != m_unusedList.size() - 1) ? L", " : L"");
						}
						std::wcout << L"]" << std::endl;
#endif

						if (!m_unusedList.size()) {
							l_isResult = true;
							return;
						}

						random_device rd;
						mt19937 random(rd());
						uniform_int_distribution<size_t> dist(0, m_unusedList.size() - 1);

						l_result[j] = m_unusedList[dist(random)];
						l_usedRowList[i].push_back(l_result[j]);
						l_usedColList[j].push_back(l_result[j]);
						l_usedChildList[3 * (i / 3) + j / 3].push_back(l_result[j]);
					}
				})();

				if (l_isResult) {
					continue;
				}

				([&]() {
					if (Unique(Sort(l_usedRowList[i])).size() < 9) {
						l_isResult = true;
						return;
					}

					for (size_t j = 0; j < m_cols; j++) {
						if (Unique(Sort(l_usedColList[j])).size() < i + 1) {
							l_isResult = true;
							return;
						}

						if (Unique(Sort(l_usedChildList[3 * (i / 3) + j / 3])).size() < 3 * (i % 3 + 1)) {
							l_isResult = true;
							return;
						}
					}
				})();

				if (l_isResult) {
					continue;
				}
			} while (l_isResult);

			for (size_t j = 0; j < m_cols; j++) {
				m_cellList[i * m_cols + j] = l_result[j];

				m_rowList[i].push_back(l_result[j]);
				m_colList[j].push_back(l_result[j]);
				m_childList[3 * (i / 3) + j / 3].push_back(l_result[j]);
			}
		}

#if _DEBUG
		PrintCellList();
#endif
	} while (!(IsRightRowList() && IsRightColList() && IsRightChildList()));

#ifdef _WINDOWS
	LARGE_INTEGER end;
	LONGLONG residual;
	QueryPerformanceCounter(&end);
	residual = (end - start).QuadPart;
#else
	clock_t residual = clock() - start;
#endif

	PrintIsRight();

	std::wcout << endl << L"created time: " << fixed << setw(6) <<
		[&]() {
#ifdef _WINDOWS
			LARGE_INTEGER perSec;
			QueryPerformanceFrequency(&perSec);
			return ( static_cast<double>(residual) / perSec.QuadPart );
#else
			return ( static_cast<double>(residual) / CLOCKS_PER_SEC );
#endif
		}() << L" sec." << endl;
#elif VERSION == 3
	do {
		m_rowList = vector<vector<size_t>>(m_rows);
		m_colList = vector<vector<size_t>>(m_cols);
		m_childList = vector<vector<size_t>>(static_cast<size_t>(sqrt(m_rows)) * static_cast<size_t>(sqrt(m_cols)));
		m_cellList = vector<size_t>(m_rows * m_cols);

		for (size_t i = 0; i < m_rows; i++) {
			vector<size_t> l_result;
			vector<vector<size_t>> l_usedRowList, l_usedColList, l_usedChildList;
			bool l_isResult;
			size_t l_failedCounter = 0;

			do {
				l_isResult = false;

				([&]() {
					l_result = vector<size_t>(m_rows);
					l_usedRowList = vector<vector<size_t>>(m_rowList);
					l_usedColList = vector<vector<size_t>>(m_colList);
					l_usedChildList = vector<vector<size_t>>(m_childList);
				})();

				([&]() {
					for (size_t j = 0; j < m_cols; j++) {
						auto m_unusedList = ([&]() {
							vector<size_t> l_temp, l_candidate = Range(1, m_cols);

							for (auto value : {
								l_usedRowList[i], l_usedColList[j], l_usedChildList[3 * (i / 3) + j / 3]
							}) {
								l_temp.insert(l_temp.end(), value.cbegin(), value.cend());
							}

							std::sort(l_temp.begin(), l_temp.end());
							l_temp.erase(std::unique(l_temp.begin(), l_temp.end()), l_temp.end());
							for (auto value : l_temp) {
								l_candidate.erase(remove(l_candidate.begin(), l_candidate.end(), value), l_candidate.end());
							}

							return l_candidate;
						})();

#if _DEBUG
						std::wcout << L"\tlist: [";
						for (size_t i = 0; i < m_unusedList.size(); i++) {
							std::wcout << m_unusedList[i] << ((i != m_unusedList.size() - 1) ? L", " : L"");
						}
						std::wcout << L"]" << std::endl;
#endif

						if (!m_unusedList.size()) {
							l_failedCounter++;
							l_isResult = true;
							return;
						}

						random_device rd;
						mt19937 random(rd());
						uniform_int_distribution<size_t> dist(0, m_unusedList.size() - 1);

						l_result[j] = m_unusedList[dist(random)];
						l_usedRowList[i].push_back(l_result[j]);
						l_usedColList[j].push_back(l_result[j]);
						l_usedChildList[3 * (i / 3) + j / 3].push_back(l_result[j]);
					}
				})();

				if (l_isResult) {
					continue;
				}

				([&]() {
					if (Unique(Sort(l_usedRowList[i])).size() < m_cols) {
						l_failedCounter++;
						l_isResult = true;
						return;
					}

					for (size_t j = 0; j < m_cols; j++) {
						if (Unique(Sort(l_usedColList[j])).size() < i + 1) {
							l_failedCounter++;
							l_isResult = true;
							return;
						}

						if (Unique(Sort(l_usedChildList[3 * (i / 3) + j / 3])).size() < 3 * (i % 3 + 1)) {
							l_failedCounter++;
							l_isResult = true;
							return;
						}
					}
				})();

				if (l_isResult) {
					continue;
				}

				l_failedCounter = 0;
			} while (l_isResult && l_failedCounter < 3);

			if (l_failedCounter >= 3) {
				continue;
			}

			for (size_t j = 0; j < m_cols; j++) {
				m_cellList[i * m_cols + j] = l_result[j];

				m_rowList[i].push_back(l_result[j]);
				m_colList[j].push_back(l_result[j]);
				m_childList[3 * (i / 3) + j / 3].push_back(l_result[j]);
			}
		}

#if _DEBUG
		PrintCellList();
#endif
	} while (!(IsRightRowList() && IsRightColList() && IsRightChildList()));

	// 解答用にコピー
	m_cellListAnswer = m_cellList;

	// 空欄を設定する
	MakeBlank();

	l_stopWatch.Stop();

	PrintIsRight();

	std::wcout << endl
		<< L"created time: "
		<< fixed << setw(6) << l_stopWatch.GetTime()
		<< L" sec." << endl;
#endif
}

vector<size_t> NumberPlace::GetCellList(void)
{
	return m_cellList;
}

vector<size_t> NumberPlace::GetAnswerCellList(void)
{
	return m_cellListAnswer;
}

size_t NumberPlace::GetCols(void)
{
	return m_cols;
}

size_t NumberPlace::GetRows(void)
{
	return m_rows;
}

void NumberPlace::MakeBlank(void)
{
	vector<size_t> l_shuffleList;
	vector<size_t> l_shuffleTable(m_rows * m_cols);
	vector<size_t> l_unusedTable = Range(0, 8);

	for (size_t i = 0; i < static_cast<size_t>(sqrt(m_rows * m_cols)); i++) {
		random_device rd;
		mt19937 random(rd());
		uniform_int_distribution<size_t> dist(0, l_unusedTable.size() - 1);
		size_t l_value = dist(random);

		l_shuffleList.push_back(l_unusedTable[l_value]);
		l_unusedTable.erase(remove(l_unusedTable.begin(), l_unusedTable.end(), l_unusedTable[l_value]), l_unusedTable.end());
	}

	size_t l_counter = 0;
	for (auto l_index : l_shuffleList) {
		for (size_t i = 0; i < 3; i++) {
			for (size_t j = 0; j < 3; j++) {
				l_shuffleTable[(l_counter / 3 * 3 + i) * m_cols + (l_counter % 3 * 3 + j)] = l_index * 9 + i * 3 + j;
			}
		}
		l_counter++;
	}

	for (auto l_index : l_shuffleTable) {
		size_t pos = l_index;
		size_t temp = m_cellList[pos];
		m_cellList[pos] = 0;
		if (!IsBlackable(pos)) {
			m_cellList[pos] = temp;
		}
	}
}

void NumberPlace::PrintIsRight(void)
{
	std::wcout << L"is unique horizontal line.........[ " << [&]() {
		return (IsRightRowList() ? L"successful" : L"  failed  ");
	}() << L" ]" << endl;

	std::wcout << L"is unique vertical line...........[ " << [&]() {
		return (IsRightColList() ? L"successful" : L"  failed  ");
	}() << L" ]" << endl;

	std::wcout << L"is unique nine rectangle line.....[ " << [&]() {
		return (IsRightChildList() ? L"successful" : L"  failed  ");
	}() << L" ]" << endl;
}

/*
* ナンプレのセル表示を行う
*/
void NumberPlace::PrintCellList(void)
{
	vector<size_t> l_list = GetCellList();

	for (size_t i = 0; i < GetRows(); i++) {
		std::wcout << L"+---+---+---+---+---+---+---+---+---+" << std::endl;
		for (size_t j = 0; j < GetCols(); j++) {
			std::wcout << L"| " << (l_list[i * GetCols() + j] ? to_string(l_list[i * GetCols() + j]).c_str() : " ") << L" ";
		}
		std::wcout << L"|" << std::endl;
	}
	std::wcout << L"+---+---+---+---+---+---+---+---+---+" << std::endl;
}

bool NumberPlace::IsRightRowList(void)
{
	bool l_isResult = true;

	for (auto rowList : m_rowList) {
		if (Unique(Sort(rowList)).size() < 9) {
			l_isResult = false;
			break;
		}
	}

	return l_isResult;
}

bool NumberPlace::IsRightColList(void)
{
	bool l_isResult = true;

	for (auto colList : m_colList) {
		if (Unique(Sort(colList)).size() < 9) {
			l_isResult = false;
			break;
		}
	}

	return l_isResult;
}

bool NumberPlace::IsRightChildList(void)
{
	bool l_isResult = true;

	for (auto childList : m_childList) {
		if (Unique(Sort(childList)).size() < 9) {
			l_isResult = false;
			break;
		}
	}

	return l_isResult;
}

bool NumberPlace::IsBlackable(size_t pos)
{
	size_t l_row = pos / m_cols;
	size_t l_col = pos % m_cols;
	vector<size_t> l_usedList = ([&]() {
		vector<size_t> l_result;
		for (size_t i = 0; i < m_cols; i++) {
			l_result.push_back(m_cellList[l_row * m_cols + i]);
		}
		for (size_t i = 0; i < m_rows; i++) {
			l_result.push_back(m_cellList[i * m_cols + l_col]);
		}
		for (size_t i = 0; i < m_rows; i++) {
			l_result.push_back(m_cellList[(l_row / 3 * 3 + i / 3) * m_cols + (l_col / 3 * 3 + i % 3)]);
		}
		l_result.erase(remove(l_result.begin(), l_result.end(), 0), l_result.end());
		return Unique(Sort(l_result));
	})();

	return l_usedList.size() == 8;
}