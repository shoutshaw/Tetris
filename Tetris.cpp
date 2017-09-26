#include "Tetris.h"

float CTetris::ComputeScore(char gamePad[COLNUM][ROWNUM], int x, int y, int shapeType, int rotateState)
{
    static char tmpGamePad[COLNUM][ROWNUM];
    int i, j;
    for (i = 0; i < COLNUM; i++)
    {
        for (j = 0; j < ROWNUM; j++)
        {
            tmpGamePad[i][j] = gamePad[i][j];
        }
    }
    for (i = 0; i < 4; i++)
    {
        tmpGamePad[x + gAstBrick[shapeType][rotateState][i].dx][y + gAstBrick[shapeType][rotateState][i].dy] = '#';
    }
    float result = (-4.500158825082766) * LandingHeight(x, y, shapeType, rotateState)
                   + (3.4181268101392694) * RowsEliminated(tmpGamePad, x, y, shapeType, rotateState)
                   + (-3.2178882868487753) * RowTransitions(tmpGamePad)
                   + (-9.348695305445199) * ColTransitions(tmpGamePad)
                   + (-7.899265427351652) * EmptyHoles(tmpGamePad)
                   + (-3.3855972247263626) * WellNums(tmpGamePad);
    return result;
}

//计算该方块已经填满的高度
int CTetris::LandingHeight(int x, int y, int shapeType, int rotateState)
{
    int maxY = 0;
    for (int i = 0; i < 4; i++)
    {
        if (y + gAstBrick[shapeType][rotateState][i].dy > maxY)
        {
            maxY = y + gAstBrick[shapeType][rotateState][i].dy;
        }
    }
    return maxY;
}

int CTetris::RowsEliminated(char gamePad[COLNUM][ROWNUM], int x, int y, int shapeType, int rotateState)
{
    int eliminatedNum = 0;
    int eliminatedGridNum = 0;
    for (int j = 0; j < ROWNUM; j++)
    {
        bool flag = true;
        for (int i = 0; i < COLNUM; i++)
        {
            if (gamePad[i][j] == '.')
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            eliminatedNum++;
            for (int s = 0; s < 4; s++)
            {
                int tmpx = x + gAstBrick[shapeType][rotateState][s].dx;
                int tmpy = y + gAstBrick[shapeType][rotateState][s].dy;
                if (tmpy == j && gamePad[tmpx][tmpy] == '#')
                {
                    eliminatedGridNum++;
                }
            }
        }
    }
    return eliminatedNum * eliminatedGridNum;
}

int CTetris::RowTransitions(char gamePad[COLNUM][ROWNUM])
{
    int totalTransNum = 0;
    for (int j = 0; j < ROWNUM; j++)
    {
        int nowTransNum = 0;
        int prevBlock = '#';
        for (int i = 0; i < COLNUM; i++)
        {
            if (gamePad[i][j] != prevBlock)
            {
                nowTransNum++;
                prevBlock = gamePad[i][j];
            }
        }
        if (prevBlock == '.')
        {
            nowTransNum++;
        }
        totalTransNum += nowTransNum;
    }
    return totalTransNum;
}

//列变换个数
int CTetris::ColTransitions(char gamePad[COLNUM][ROWNUM])
{
    int totalTransNum = 0;
    for (int i = 0; i < COLNUM; i++)
    {
        int nowTransNum = 0;
        int prevBlock = '#';
        for (int j = 0; j < ROWNUM; j++)
        {
            if (gamePad[i][j] != prevBlock)
            {
                nowTransNum++;
                prevBlock = gamePad[i][j];
            }
        }
        if (prevBlock == '.')
        {
            nowTransNum++;
        }
        totalTransNum += nowTransNum;
    }
    return totalTransNum;
}

//空洞个数
int CTetris::EmptyHoles(char gamePad[COLNUM][ROWNUM])
{
    int totalEmptyHoles = 0;
    for (int i = 0; i < COLNUM; i++)
    {
        int j = ROWNUM - 1;
        int emptyHoles = 0;
        for (; j >= 0; j--)
        {
            if (gamePad[i][j] == '#')
            {
                j--;
                break;
            }
        }
        for (; j >= 0; j--)
        {
            if (gamePad[i][j] == '.')
            {
                emptyHoles++;
            }
        }
        totalEmptyHoles += emptyHoles;
    }
    return totalEmptyHoles;
}

//井的个数
int CTetris::WellNums(char gamePad[COLNUM][ROWNUM])
{
    int i = 0, j = 0, wellDepth = 0, tDepth = 0;
    int totalWellDepth = 0;
    // *) 获取最左边的井数
    wellDepth = 0;
    tDepth = 0;
    for (j = ROWNUM - 1; j >= 0; j--)
    {
        if (gamePad[0][j] == '.' && gamePad[1][j] == '#')
        {
            tDepth++;
        }
        else
        {
            wellDepth += tDepth * (tDepth + 1) / 2;
            tDepth = 0;
        }
    }
    wellDepth += tDepth * (tDepth + 1) / 2;
    totalWellDepth += wellDepth;
    // *) 获取中间的井数
    wellDepth = 0;
    for (i = 1; i < COLNUM - 1; i++)
    {
        tDepth = 0;
        for (j = ROWNUM - 1; j >= 0; j--)
        {
            if (gamePad[i][j] == '.' && gamePad[i - 1][j] == '#' && gamePad[i + 1][j] == '#')
            {
                tDepth++;
            }
            else
            {
                wellDepth += tDepth * (tDepth + 1) / 2;
                tDepth = 0;
            }
        }
        wellDepth += tDepth * (tDepth + 1) / 2;
    }
    totalWellDepth += wellDepth;
    // *) 获取最右边的井数
    wellDepth = 0;
    tDepth = 0;
    for (j = ROWNUM - 1; j >= 0; j--)
    {
        if (gamePad[COLNUM - 1][j] == '.' && gamePad[COLNUM - 2][j] == '#')
        {
            tDepth++;
        }
        else
        {
            wellDepth += tDepth * (tDepth + 1) / 2;
            tDepth = 0;
        }
    }
    wellDepth += tDepth * (tDepth + 1) / 2;
    totalWellDepth += wellDepth;
    return totalWellDepth;
}

string CTetris::MakeKey(int x, int y, int rotateState)
{
    string key = to_str<int>(x) + ":" + to_str<int>(y) + ":" + to_str<int>(rotateState);
    return key;
}

bool CTetris::HasKey(string key)
{
    map<string, bool>::iterator it = mKeyMap.find(key);
    if (it == mKeyMap.end())
    {
        mKeyMap[key] = true;
        return false;
    }
    else
    {
        return true;
    }
}

bool CTetris::CheckAvailable(int x, int y, int shapeType, int rotateState)
{
    for (int i = 0; i < 4; i++)
    {
        tPoint temp = gAstBrick[shapeType][rotateState][i];
        tPoint tempPoint;
        tempPoint.dx = x + temp.dx;
        tempPoint.dy = y + temp.dy;

        //tempPoint.dy > ROWNUM 这种情况可以超过上边界
        if (tempPoint.dx < 0 || tempPoint.dx > COLNUM || tempPoint.dy < 0)
        {
            return false;
        }
        if (mGamePad[tempPoint.dx][tempPoint.dy] == '#')
        {
            return false;
        }
    }
    return true;
}

int CTetris::Evaluate(char gamePad[COLNUM][ROWNUM], int shapeType)
{
    for (int i = 0; i < COLNUM; i++)
    {
        for (int j = 0; j < ROWNUM; j++)
        {
            mGamePad[i][j] = gamePad[i][j];
        }
    }
    SShapeState initShapeState(OP_INIT, -1, 5, 29, 0);
    vector<SShapeState> stepVec;
    stepVec.push_back(initShapeState);
    int vecIdx = 0;
    while (vecIdx < stepVec.size())
    {
        SShapeState curStep = stepVec[vecIdx];
        int x, y, roteState;
        
        //left
        x = curStep.mX - 1;
        y = curStep.mY;
        roteState = curStep.mShapeRotateState;
        if (CheckAvailable(x, y, shapeType, roteState))
        {
            if (!HasKey(MakeKey(x, y, roteState)))
            {
                SShapeState nextStep(OP_LEFT, vecIdx, x, y, roteState);
                stepVec.push_back(nextStep);
            }
        }
        
        //right
        x = curStep.mX + 1;
        y = curStep.mY;
        roteState = curStep.mShapeRotateState;
        if (CheckAvailable(x, y, shapeType, roteState))
        {
            if (!HasKey(MakeKey(x, y, roteState)))
            {
                SShapeState nextStep(OP_RIGHT, vecIdx, x, y, roteState);
                stepVec.push_back(nextStep);
            }
        }
        
        //crote
        x = curStep.mX;
        y = curStep.mY;
        roteState = (curStep.mShapeRotateState + 1) % 4;
        if (CheckAvailable(x, y, shapeType, roteState))
        {
            if (!HasKey(MakeKey(x, y, roteState)))
            {
                SShapeState nextStep(OP_CROTE, vecIdx, x, y, roteState);
                stepVec.push_back(nextStep);
            }
        }
        
        //arote
        x = curStep.mX;
        y = curStep.mY;
        roteState = (curStep.mShapeRotateState - 1 + 4) % 4; //+4 防止越界
        if (CheckAvailable(x, y, shapeType, roteState))
        {
            if (!HasKey(MakeKey(x, y, roteState)))
            {
                SShapeState nextStep(OP_AROTE, vecIdx, x, y, roteState);
                stepVec.push_back(nextStep);
            }
        }
        
        //down
        x = curStep.mX;
        y = curStep.mY - 1;
        roteState = curStep.mShapeRotateState;
        if (CheckAvailable(x, y, shapeType, roteState))
        {
            if (!HasKey(MakeKey(x, y, roteState)))
            {
                SShapeState nextStep(OP_DOWN, vecIdx, x, y, roteState);
                stepVec.push_back(nextStep);
            }
        }
        else
        {
            vector<int> roadVec;
            roadVec.push_back(curStep.mOp);
            int prevStep = curStep.mPrev;
            while (prevStep != -1)
            {
                roadVec.push_back(stepVec[prevStep].mOp);
                prevStep = stepVec[prevStep].mPrev;
            }
            mFinalState.push_back(curStep);
            mFinalStep.push_back(roadVec);
        }
        
        vecIdx++;
    }

    for (int k = 0; k < mFinalState.size(); k++)
    {
        float score = ComputeScore(mGamePad, mFinalState[k].mX, mFinalState[k].mY, shapeType, mFinalState[k].mShapeRotateState);
        mFinalScore.push_back(score);
    }
}

int main()
{
    /*for(int shapeType = 0; shapeType < 7; shapeType++)
    {
        for(int roteState = 0; roteState < 4; roteState++)
        {
            char gamePad[COLNUM][ROWNUM];

            for(int i = 0; i < COLNUM; i++)
            {
                for(int j = 0; j < ROWNUM; j++)
                {
                    gamePad[i][j] = '.';
                }
            }

            for(int i = 0; i < 4; i++)
            {
                tPoint temp = gAstBrick[shapeType][roteState][i];
                tPoint tempPoint;
                tempPoint.dx = focus.dx + temp.dx;
                tempPoint.dy = focus.dy + temp.dy;
                if(tempPoint.dx < 0 || tempPoint.dx > COLNUM || tempPoint.dy < 0 || tempPoint.dy > ROWNUM)
                {
                    cout << "ivalid point" << endl;
                    return -1;
                }
                else
                {
                    gamePad[tempPoint.dx][tempPoint.dy] = '#';
                }
            }

            for(int j = (ROWNUM - 1); j >= 0; j--)
            {
                for(int i = 0; i < COLNUM; i++)
                {
                    cout << gamePad[i][j];
                }
                cout << endl;
            }
            cout << endl;
        }
    }*/
    char gamePad[COLNUM][ROWNUM];
    for (int j = 0; j < ROWNUM; j++)
    {
        for (int i = 0; i < COLNUM; i++)
        {
            gamePad[i][j] = '.';
        }
    }
    gamePad[0][0] = '#';
    gamePad[1][0] = '#';
    gamePad[2][0] = '#';
    gamePad[3][0] = '#';
    gamePad[4][0] = '#';
    gamePad[5][0] = '.';
    gamePad[6][0] = '.';
    gamePad[7][0] = '#';
    gamePad[8][0] = '#';
    gamePad[9][0] = '#';
    gamePad[10][0] = '#';
    gamePad[11][0] = '#';
    gamePad[0][1] = '#';
    gamePad[1][1] = '#';
    gamePad[2][1] = '#';
    gamePad[3][1] = '#';
    gamePad[4][1] = '.';
    gamePad[5][1] = '.';
    gamePad[6][1] = '.';
    gamePad[7][1] = '.';
    gamePad[8][1] = '#';
    gamePad[9][1] = '#';
    gamePad[10][1] = '#';
    gamePad[11][1] = '#';
    CTetris* tetris = new CTetris;
    float maxScore;
    int maxIdx;
    tetris->Evaluate(gamePad, STYPE);
    for (int i = 0; i < tetris->mFinalScore.size(); i++)
    {
        float score = tetris->mFinalScore[i];
        if (i == 0)
        {
            maxScore = score;
            maxIdx = i;
        }
        if (score < maxScore)
        {
            maxScore = score;
            maxIdx = i;
        }
        cout << "maxScore: " << maxScore << " score: " << score << endl;
    }
    vector<int>::reverse_iterator it = tetris->mFinalStep[maxIdx].rbegin();
    for (; it !=  tetris->mFinalStep[maxIdx].rend(); it++)
    {
        int op = *it;
        cout << " Op: " << op;
        /*char tGamePad[COLNUM][ROWNUM];
        for(int j = 0; j < ROWNUM; j++)
        {
            for(int i = 0; i < COLNUM; i++)
            {
                tGamePad[i][j] = gamePad[i][j];
            }
        }



        for(int j = (ROWNUM - 1); j >= 0; j--)
        {
            for(int i = 0; i < COLNUM; i++)
            {
                cout << tGamePad[i][j];
            }
            cout << endl;
        }
        cout << endl;
        */
    }
    cout << endl;
    return 0;
}
