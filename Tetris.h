#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

const int ROWNUM = 30;
const int COLNUM = 12;

struct tPoint
{
    int dx;   // 负为左，正为右，实际上x坐标对应到列
    int dy;   // 负为下，正为上，实际上y坐标对应到行
};

static const tPoint gAstBrick[7][4][4]  =
{
    {
        // T型
        {{0, 0}, { -1, 0}, {1, 0}, {0, -1}},
        {{0, 0}, { -1, 0}, {0, 1}, {0, -1}},
        {{0, 0}, { -1, 0}, {0, 1}, {1, 0}},
        {{0, 0}, {0, -1}, {0, 1}, {1, 0}}
    },
    {
        // I型
        {{0, 0}, {0, -1}, {0, 1}, {0, 2}},
        {{0, 0}, { -1, 0}, {1, 0}, {2, 0}},
        {{0, 0}, {0, -1}, {0, 1}, {0, 2}},
        {{0, 0}, { -1, 0}, {1, 0}, {2, 0}}
    },
    {
        // S型
        {{0, 0}, { -1, 0}, {0, 1}, {1, 1}},
        {{0, 0}, {0, 1}, {1, 0}, {1, -1}},
        {{0, 0}, { -1, 0}, {0, 1}, {1, 1}},
        {{0, 0}, {0, 1}, {1, 0}, {1, -1}}
    },
    {
        // Z型
        {{0, 0}, { -1, 1}, {0, 1}, {1, 0}},
        {{0, 0}, { -1, -1}, {0, 1}, { -1, 0}},
        {{0, 0}, { -1, 1}, {0, 1}, {1, 0}},
        {{0, 0}, { -1, -1}, {0, 1}, { -1, 0}}
    },
    {
        // O型
        {{0, 0}, {1, 1}, {0, 1}, {1, 0}},
        {{0, 0}, {1, 1}, {0, 1}, {1, 0}},
        {{0, 0}, {1, 1}, {0, 1}, {1, 0}},
        {{0, 0}, {1, 1}, {0, 1}, {1, 0}}
    },
    {
        // L型
        {{0, 0}, {0, 1}, {0, 2}, {1, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {0, -1}},
        {{0, 0}, {0, -1}, {0, -2}, { -1, 0}},
        {{0, 0}, { -1, 0}, { -2, 0}, {0, 1}}
    },
    {
        // J型
        {{0, 0}, {0, 1}, {0, 2}, { -1, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {0, 1}},
        {{0, 0}, {0, -1}, {0, -2}, {1, 0}},
        {{0, 0}, { -1, 0}, { -2, 0}, {0, -1}}
    }
};

enum ShapeType
{
    TTYPE = 0,
    ITYPE = 1,
    STYPE = 2,
    ZTYPE = 3,
    OTYPE = 4,
    LTYPE = 5,
    JTYPE = 6
};

enum Op
{
    OP_INIT = 0,
    OP_LEFT = 1,
    OP_RIGHT = 2,
    OP_DOWN = 3,
    OP_CROTE = 4,
    OP_AROTE = 5,
};

struct SShapeState
{
    SShapeState(int op, int prev, int x, int y, int rotateState):
        mOp(op), mPrev(prev), mX(x), mY(y), mShapeRotateState(rotateState) {}

    int mOp;
    int mPrev;
    int mX;
    int mY;
    int mShapeRotateState;
};

template <typename T>
inline T from_str(const std::string& str)
{
    std::istringstream is(str);
    T t;
    is >> t;
    return t;
}

template <typename T>
inline std::string to_str(const T& t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}


class CTetris
{
    public:
        CTetris()
        {

        }
        ~CTetris() {}

        int Evaluate(char gamePad[COLNUM][ROWNUM], int shapeType);

        vector<SShapeState> mFinalState; //最终的情况
        vector< vector<int> > mFinalStep; //最终情况的路径
        vector<float> mFinalScore;

    private:
        bool CheckAvailable(int x, int y, int shapeType, int rotateState);
        string MakeKey(int x, int y, int rotateState);
        bool HasKey(string key);
        float compute_score(char gamePad[COLNUM][ROWNUM], int x, int y, int shapeType, int rotateState);
        int LandingHeight(int x, int y, int shapeType, int shapeState);
        int RowsEliminated(char gamePad[COLNUM][ROWNUM], int x, int y, int shapeType, int rotateState);
        int RowTransitions(char gamePad[COLNUM][ROWNUM]);
        int ColTransitions(char gamePad[COLNUM][ROWNUM]);
        int EmptyHoles(char gamePad[COLNUM][ROWNUM]);
        int WellNums(char gamePad[COLNUM][ROWNUM]);


        char mGamePad[COLNUM][ROWNUM]; //当前盘面
        map<string, bool> mKeyMap;
};

