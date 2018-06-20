
int main()
{
    try
    {
        return __builtin_return_empty();
    }
    catch (int i)
    {
        return i;
    }
    catch (...)
    {
        return 0;
    }
}
