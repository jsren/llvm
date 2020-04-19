int other(int i)
{
    int vla[i];
    return vla[1];
}

int main()
{
    return other(5);
}
