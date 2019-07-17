#include<stdio.h>
#include <stack>
#include <assert.h>
#include <fstream>
using namespace std;
#define SZ 100005
long long ml[4*SZ],tree[4*SZ],cnta[4*SZ],cntb[4*SZ],x[4*SZ], arr[SZ+5], S;
bool setx[4*SZ];

void build(long long l,long long h,long long pos)
{
    if(l==h)
    {
        setx[pos]=0;//determines x is set or not here
        x[pos]=0LL;// given x value for the range.
        tree[pos]=0LL;// update of x in upward flow.
        ml[pos]=0LL;// extra value of 1 to n series .. eg. 3+4+5 which means 2*3+(1+2+3) here ml[pos]=2*3 to make the series 1 to n..
        cnta[pos]=0LL;// number of 1 to n series..
        cntb[pos]=0LL;
        return;
    }

    build(l,(l+h)/2,pos*2);
    build((l+h)/2+1,h,pos*2+1);

    setx[pos]=0;
    x[pos]=0LL;
    tree[pos]=tree[pos*2]+tree[pos*2+1];
    ml[pos]=0LL;
    cnta[pos]=cntb[pos]=0LL;
}

void flow(long long posl,long long posh,long long pos)
{
    if(setx[pos])
    {
        setx[pos*2]=setx[pos*2+1]=1,x[pos*2]=x[pos*2+1]=x[pos],setx[pos]=0;

        ml[pos*2]=ml[pos*2+1]=0LL;
        cnta[pos*2]=cnta[pos*2+1]=0LL;
        cntb[pos*2]=cntb[pos*2+1]=0LL;
    }
    ml[pos*2]+=ml[pos];
    ml[pos*2+1]+=ml[pos];
    ml[pos]=0;

    cntb[pos*2]+=cntb[pos];
    cntb[pos*2+1]+=cntb[pos];
    ml[pos*2]+=cntb[pos]*(posh-((posh+posl)/2));
    cntb[pos]=0;

    cnta[pos*2]+=cnta[pos];
    cnta[pos*2+1]+=cnta[pos];
    ml[pos*2+1]+=cnta[pos]*(((posh+posl)/2)+1-posl);
    cnta[pos]=0;
}

long long upA(long long dir,long long l,long long h,long long posl,long long posh,long long pos)
{
    long long n,nn;
    n=(posh-posl)+1LL;
    nn=(n*(n+1))/2LL;

    if(l<=posl && h>=posh)
    {
        if(dir==0)
            cnta[pos]++;
        else
            cntb[pos]++;

        if(dir==0)
            ml[pos]+=(posl-l);
        else
            ml[pos]+=(h-posh);

        if(setx[pos])
            return x[pos]*n+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
        else
            return tree[pos]+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
    }
    else if(l>posh || h<posl)
    {
        if(setx[pos])
            return x[pos]*n+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
        else
            return tree[pos]+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
    }

    flow(posl,posh,pos);

    tree[pos]=upA(dir,l,h,posl,(posl+posh)/2,pos*2)+upA(dir,l,h,(posl+posh)/2+1,posh,pos*2+1);

    return tree[pos]+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
}

long long upC(long long l,long long h,long long posl,long long posh,long long pos,long long xy)
{
    long long n,nn;
    n=(posh-posl)+1LL;
    nn=(n*(n+1))/2LL;

    if(l<=posl && h>=posh)
    {
        setx[pos]=1,x[pos]=xy;
        ml[pos]=cnta[pos]=cntb[pos]=0LL;
        return x[pos]*n;
    }
    else if(l>posh || h<posl)
    {
        if(setx[pos])
            return x[pos]*n+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
        else
            return tree[pos]+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
    }

    flow(posl,posh,pos);

    tree[pos]=upC(l,h,posl,(posl+posh)/2,pos*2,xy)+upC(l,h,(posl+posh)/2+1,posh,pos*2+1,xy);

    return tree[pos]+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
}

long long upD(long long l,long long h,long long posl,long long posh,long long pos,long long xy)
{
    long long n,nn;
    n=(posh-posl)+1LL;
    nn=(n*(n+1))/2LL;

    if(l<=posl && h>=posh)
    {
        ml[pos] += xy;
        if(setx[pos])
            return x[pos]*n+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
        else
            return tree[pos]+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
    }
    else if(l>posh || h<posl)
    {
        if(setx[pos])
            return x[pos]*n+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
        else
            return tree[pos]+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
    }

    flow(posl,posh,pos);

    tree[pos]=upD(l,h,posl,(posl+posh)/2,pos*2,xy)+upD(l,h,(posl+posh)/2+1,posh,pos*2+1,xy);

    return tree[pos]+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
}

long long pclean(long long l,long long h,long long posl,long long posh,long long pos)
{
    long long n,nn;
    n=(posh-posl)+1LL;
    nn=(n*(n+1))/2LL;

    if((l<=posl && h>=posh) || (l>posh || h<posl))
    {
        if(setx[pos])
            return x[pos]*n+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
        else
            return tree[pos]+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
    }

    flow(posl,posh,pos);

    return tree[pos]=pclean(l,h,posl,(posl+posh)/2,pos*2)+pclean(l,h,(posl+posh)/2+1,posh,pos*2+1);
}

long long getSum(long long l, long long h, long long posl, long long posh, long long pos)
{
    if(l<=posl && h>=posh)
    {
        long long n,nn;
        n=(posh-posl)+1LL;
        nn=(n*(n+1))/2LL;

        if(setx[pos])
            return x[pos]*n+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
        else
            return tree[pos]+nn*(cnta[pos]+cntb[pos])+ml[pos]*n;
    }
    else if(l>posh || h<posl)
        return 0LL;

    return getSum(l,h,posl,(posl+posh)/2,pos*2)+getSum(l,h,(posl+posh)/2+1,posh,pos*2+1);
}

void shovel(long long c,long long w)
{
    long long l = c - w + 1;
    long long h = c + w - 1;
    if (l < 1) {
        pclean(1, c, 0, S, 1);
        upD(1, c, 0, S, 1, 1 - l);
        l = 1;
    }

    if (h > S) {
        pclean(c + 1, S, 0, S, 1);
        upD(c + 1, S, 0, S, 1, h - S);
        h = S;
    }

    upA(0, l, c, 0, S, 1);
    upA(1, c + 1, h, 0, S, 1);
}

void hammer(long long c, long long w) {

    long long l = c - w + 1LL;
    long long h = c + w - 1LL;
    if (l < 1) {
        l = 1;
    }
    if (h > S) {
        h = S;
    }
    pclean(l,h,0,S,1);
    long long avg = getSum(l, h, 0, S, 1)/(h-l+1LL);
    upC(l,h,0,S,1,avg);
}

void flowAll(long long posl,long long posh,long long pos)
{
    if (posl == posh) {
        return;
    }

    flow(posl,posh,pos);

    flowAll(posl,(posl+posh)/2,pos*2);
    flowAll((posl+posh)/2+1,posh,pos*2+1);
}

long long getMaxRectArea()
{
    flowAll(0, S, 1);

    for (int i = 0; i <S; i++) {
        pclean(i,i,0,S,1);
        arr[i] = getSum(i+1, i+1, 0, S, 1);
    }

    stack<int> st;
    int i, j;
    long long maxarea = 0, area;
    i = 0;
    while(i < S)
    {
        if(st.empty() || arr[st.top()] <= arr[i])
        {
            st.push(i++);
            continue;
        }

        j = st.top();
        st.pop();
        area = arr[j] * (long long)((st.empty() ? i : i - st.top() - 1));
        if(area > maxarea) maxarea = area;
    }
    while(st.size())
    {
        j = st.top();
        st.pop();
        area = arr[j] * (long long)((st.empty() ? i : i - st.top() - 1));
        if(area > maxarea) maxarea = area;
    }
    return maxarea;
}

int main()
{
    //freopen("data01.in", "r", stdin);
    //freopen("out01.txt", "w", stdout);
    long long q,l,h;
    char st[3];
    scanf("%lld%lld", &S, &q);
    assert(1 <= S && S <= 100000);
    build(0,S,1);

    while(q--)
    {
        scanf("%s%lld%lld",&st, &l, &h);
        assert(1 <= l && l <= 100000);
        assert(1 <= h && h <= 100000);

        if(st[0]=='S')
        {
            shovel(l,h);
        }
        else if(st[0]=='H')
        {
            hammer(l, h);
        }
        else if(st[0]=='A')
        {
            assert(l <= h);
            pclean(l,h,0,S,1);
            printf("%lld\n", getSum(l, h, 0, S, 1));
        }
    }
    printf("effective area : %lld\n", getMaxRectArea());
    return 0;
}