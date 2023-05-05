// khai báo sử dụng thư viện mẫu của Windows - cần thiết cho tất cả các ứng dụng
#include <windows.h>
#include <d3d9.h>

HINSTANCE hInst; // khai báo biến toàn cục chứa con trỏ instance (biến thể) của chương trình
HWND wndHandle; // biến toàn cục chứa con trỏ quản lý cửa sổ ứng dụng
// khai báo hàm
bool initWindow(HINSTANCE hInstance);

bool initDirect3D(void);

void render(void);

void cleanUp (void);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Đây là hàm winmain, hàm được gọi đầu tiền của mọi ứng dụng trong window
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPTSTR lpCmdLine, int nCmdShow) {
// khởi tạo cửa sổ ứng dụng
    if (!initWindow(hInstance))
        return false;
    // hàm này được gọi sau khi quá trình khởi tạo cửa sổ ứng dụng kết thúc
    if (!initDirect3D())
        return false;
// vòng lặp chính dùng để quản lý thông điệp:
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
// Kiểm tra các sự kiện được gửi tới trong hàng đợi của ứng dụng
//        while (GetMessage(&msg, wndHandle, 0, 0)) {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage ( &msg );
            DispatchMessage ( &msg );
        }
        else
        {
            render( );
        }
    }
    return (int) msg.wParam;
}

/******************************************************************************
* bool initWindow( HINSTANCE hInstance )
* initWindow registers the window class for the application, creates the window
******************************************************************************/
bool initWindow( HINSTANCE hInstance )
{
    WNDCLASSEX wcex;
// Xác lập thuộc tính đối tượng kiểu WNDCLASSEX structure. Các thuộc tính này sẽ tác
// động tới cách thể hiện của cửa sổ chương trình
    wcex.cbSize = sizeof(WNDCLASSEX); // hàm sizeof() trả về kích thước của một đối
//tượng kiểu dữ liệu đầu vào – đơn vị tính là byte
    wcex.style = CS_HREDRAW | CS_VREDRAW; // xác lập kiểu lớp
    wcex.lpfnWndProc = (WNDPROC)WndProc; // xác lập tên hàm gọi lại callback procedure
    wcex.cbClsExtra = 0; // xác lập số byte cấp phát thêm cho Class
    wcex.cbWndExtra = 0; // xác lập số byte cấp phát thêm cho mỗi instance của Class
    wcex.hInstance = hInstance; // con trỏ (handle) trỏ tới instance của ứng dụng
    wcex.hIcon = 0; //loại biểu tượng chương trình
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);// xác lập kiểu con trỏ chuột mặc định
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // màu nền của cửa sổ
    wcex.lpszMenuName = nullptr; // con trỏ trỏ tới object dữ liệu thực đơn ứng dụng
    wcex.lpszClassName = "DirectXExample"; // tên lớp đăng ký với hệ thống
    wcex.hIconSm = 0; // con trỏ tới dữ liệu biểu tượng cửa sổ ứng dụng
    RegisterClassEx(&wcex); //gọi hàm đăng ký lớp với hệ thống
// Tạo cửa sổ mới
    wndHandle = CreateWindow(
            "DirectXExample", //tên lớp sử dụng đã khai báo và đăng ký ở trên
            "DirectXExample", //tiêu đề của cửa sổ chương trình
            WS_OVERLAPPEDWINDOW, //loại của sổ chương trình
            CW_USEDEFAULT, // toạ độ X của của sổ khi xuất hiện
            CW_USEDEFAULT, // toạ độ Y của của sổ khi xuất hiện
            640, // kích thước bề ngang của cửa sổ - đơn vị là pixel
            480, // kích thước chiều cao của cửa sổ
            nullptr, // con trỏ trỏ tới đối tượng cha ;
//NULL = đối tượng quản lý là desktop của Windows
            nullptr, // con trỏ đối tượng menu của chương trình; NULL = không sử dụng
            hInstance, // con trỏ instance của ứng dụng
            nullptr); // không có giá trị gì được truyền cho cửa sổ
// Kiểm tra lại xem quá trinh khởi tạo cửa sổ có thành công hay không
    if (!wndHandle)
        return false;
// Thể hiện cửa sổ lên màn hình Window
    ShowWindow(wndHandle, SW_SHOW);
    UpdateWindow(wndHandle);
    return true;
}

/******************************************************************************
* LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
* LPARAM lParam)
* The window procedure
******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam)
{
// Kiểm tra xem có thông điệp nào được gửi tới hàng đợi cửa ứng dụng không
    switch (message) //lọc các thông điệp
    {
        case WM_DESTROY: //bắt thông điệp yêu cầu kết thúc ứng dụng
            PostQuitMessage(0); //gọi hàm xử lý
            break;
    }
// Chú ý, hàm này yêu cầu bạn phải cung cấp thông điệp trả về cho quá trình
// xứ lý tiếp theo
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LPDIRECT3D9 pD3D; //Đối tượng Direct3D
LPDIRECT3DDEVICE9 pd3dDevice; //thiết bị hiển thị Direct3D

/*********************************************************************
* initDirect3D
*********************************************************************/
bool initDirect3D(void)
{
    pD3D = NULL;
    pd3dDevice = NULL;
// Create the DirectX object
    if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
    {
        return false;
    }
// Fill the presentation parameters structure
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferHeight = 480;
    d3dpp.BackBufferWidth = 640;
    d3dpp.hDeviceWindow = wndHandle;
// Create a default DirectX device
    if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT,
                                    D3DDEVTYPE_REF,
                                    wndHandle,
                                    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                    &d3dpp,
                                    &   pd3dDevice ) ) )
    {
        return false;
    }
    return true;
}

/*********************************************************************
* render
*********************************************************************/
void render(void)
{
// Kiểm tra xem chắc chắn thiết bị Direct3D đã được tạo hay chưa
    if( NULL == pd3dDevice )
        return;// Xoá toàn bộ bộ đệm màn hình về trạng thái mầu xanh da trời
    pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
                       D3DCOLOR_XRGB( 0,0,255 ), 1.0f, 0 );
// Thể hiện dữ liệu bộ đệm lên màn hình
    pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void cleanUp (void)
{
// Giải phóng đối tượng và thiết bị hiển thị DIrect3D
    if( pd3dDevice != NULL )
        pd3dDevice->Release( );
    if( pD3D != NULL )
        pD3D->Release( );
}