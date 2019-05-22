#include<vector>
#include<array>

using namespace std;

class NumberPlace
{
private:
	size_t					m_cols;
	size_t					m_rows;
	vector<vector<size_t>>	m_colList;
	vector<vector<size_t>>	m_rowList;
	vector<vector<size_t>>	m_childList;
	vector<size_t>			m_cellList;
	vector<size_t>			m_cellListAnswer;

public:
	NumberPlace(size_t p_width = 9, size_t p_height = 9);

	void					Create(void);

	vector<size_t>			GetCellList(void);
	vector<size_t>			GetAnswerCellList(void);
	size_t					GetCols(void);
	size_t					GetRows(void);

	void					PrintCellList(void);

private:
	void					MakeBlank(void);

	void					PrintIsRight(void);

	bool					IsRightRowList(void);
	bool					IsRightColList(void);
	bool					IsRightChildList(void);

	bool					IsBlackable(size_t pos);
};