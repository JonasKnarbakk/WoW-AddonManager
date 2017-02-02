#ifndef RETURNVALUE_HPP
#define RETURNVALUE_HPP

class ReturnValue{
    private:

    public:
        ReturnValue();
        ~ReturnValue();

        // Checks that the return value of the method is not a nullptr
        bool notNull(void * ptr);

        // Compares return values with expected values
        bool expectedFloat(float &value, float &expected);
        bool expectedInteger(int &value, int &expected);
        bool expectedDouble(double &value, double &expected);
        bool expectedBool(bool &value, bool &expected);
        
};

#endif
