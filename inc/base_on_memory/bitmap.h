#ifndef BITMAP_H_
#define BITMAP_H_
// Bit Set
class Bitmap {
    public:
    explicit Bitmap(int size) {
        this->size = size;
        n = (size+8)/8;
        assert(map = new char[n]);
    }

    ~Bitmap() { delete[] map; }

    void reset() {
        for (int i = 0; i < n; i++) map[i] = 0; }
    
    void set(int idx) {
        if (idx<0 or idx>=size) return;
        int i = idx / 8;
        int j = idx % 8;
        switch(j) {
            case 0:
                map[i] |= 0x01;
                break;
            case 1:
                map[i] |= 0x02;
                break;
            case 2:
                map[i] |= 0x04;
                break;
            case 3:
                map[i] |= 0x08;
                break;
            case 4:
                map[i] |= 0x10;
                break;
            case 5:
                map[i] |= 0x20;
                break;
            case 6:
                map[i] |= 0x40;
                break;
            default: // 7
                map[i] |= 0x80;
                break;
        }
    }

    bool test(int idx) {
        if (idx<0 or idx>=size) return false;
        int i = idx / 8;
        int j = idx % 8;
        switch(j) {
            case 0:
                return map[i] & 0x01;
                break;
            case 1:
                return map[i] & 0x02;
                break;
            case 2:
                return map[i] & 0x04;
                break;
            case 3:
                return map[i] & 0x08;
                break;
            case 4:
                return map[i] & 0x10;
                break;
            case 5:
                return map[i] & 0x20;
                break;
            case 6:
                return map[i] & 0x40;
                break;
            default: // 7
                return map[i] & 0x80;
                break;
        }
    }

    private:
    char * map;
    int   n;
    int   size;
};

#endif
