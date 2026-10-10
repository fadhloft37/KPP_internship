const int MAKS= 500000;

class people{
protected:
    int total;

public:
    people(int n){
        total=n;
    }
    virtual ~people()= default;

    void run(int q);

    virtual void panggil() = 0;
    virtual void datang(int x) = 0;
    virtual int panggilLagi() = 0;
};

class peopleCalled : public people{
private:
    int e1;
    int queue[MAKS];

public:
    peopleCalled(int n);
    void panggil() override;
    void datang(int x) override;
    int panggilLagi() override;
};