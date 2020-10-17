    //Transform functions
    template <bool CORDS_PARAMS_FLAG = false>
    constexpr FastMat Transform(const auto &Func) const
    {
        Mat res;
        for (size_t i = 0; i < H; ++i)
        {
            for (size_t j = 0; j < W; ++j)
            {
                if constexpr (CORDS_PARAMS_FLAG)
                {
                    res.members[i][j] = Func(this->members[i][j], j, i);
                }
                else
                {
                    res.members[i][j] = Func(this->members[i][j]);
                }
            }
        }
        return res;
    }
    template <bool CORDS_PARAMS_FLAG = false>
    constexpr FastMat Transform(const auto &Func, const Mat &mat) const
    {
        Mat res;
        for (size_t i = 0; i < H; ++i)
        {
            for (size_t j = 0; j < W; ++j)
            {
                if constexpr (CORDS_PARAMS_FLAG)
                {
                    res.members[i][j] = Func(this->members[i][j], mat.members[i][j], j, i);
                }
                else
                {
                    res.members[i][j] = Func(this->members[i][j], mat.members[i][j]);
                }
            }
        }
        return res;
    }

    //ApplyFunction function
    template <bool CORDS_PARAMS_FLAG = false>
    constexpr void ApplyFunction(const auto &Func)
    {
        for (size_t i = 0; i < H; ++i)
        {
            for (size_t j = 0; j < W; ++j)
            {
                if constexpr (CORDS_PARAMS_FLAG)
                {
                    Func(this->members[i][j], j, i);
                }
                else
                {
                    Func(this->members[i][j]);
                }
            }
        }
    }
    template <bool CORDS_PARAMS_FLAG = false>
    constexpr void ApplyFunction(const auto &Func, const Mat &mat)
    {
        for (size_t i = 0; i < H; ++i)
        {
            for (size_t j = 0; j < W; ++j)
            {
                if constexpr (CORDS_PARAMS_FLAG)
                {
                    Func(this->members[i][j], mat.members[i][j], j, i);
                }
                else
                {
                    Func(this->members[i][j], mat.members[i][j]);
                }
            }
        }
    }
