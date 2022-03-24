// Chấm Mực Trực tự
// 2019.11.15

// Hố đen lượng tử 2019.11.15 03:30

#define kPI_CHIA_2 1.5707963268f   // π/2
#define kPI_CHIA_4 0.7853981634f   // π/4
#define k4_TRU_PI  0.8584073464f   // 4 - π

#define kLE 10

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PNG.h"

typedef struct {
   float kho;
   float x;
   float y;
   float dienTich;
} ChamMuc;

enum {
   C,
   M,
   Y,
   K
};

// ---- khổ chấm mực --- tương đối với điểm ảnh
#define kKHO_MUC 0.5f

// ---- quét ảnh
void quetAnhGoc0_CMYK_toDen( FILE *tepSVG, float *anh, unsigned int beRongAnh, unsigned int beCaoAnh, unsigned char buoc, unsigned char soKenh, float kho );
void quetAnhGoc0_CMYK_toMau( FILE *tepSVG, float *anh, unsigned int beRongAnh, unsigned int beCaoAnh, unsigned char buoc, unsigned char soKenh, float kho, unsigned int mau, float doDuc );

void quetAnhGocKhac_CMYK_toDen( FILE *tepSVG, float *anh, unsigned int beRongAnh, unsigned int beCaoAnh, unsigned char buoc, unsigned char soKenh, float kho, float goc );
void quetAnhGocKhac_CMYK_toMau( FILE *tepSVG, float *anh, unsigned int beRongAnh, unsigned int beCaoAnh, unsigned char buoc, unsigned char soKenh, float kho, float goc, unsigned int mau, float doDuc );

// ---- biến đổi RGB sang CYMK
float *bienDoiRGBSangCMYK( unsigned char *anh, unsigned int beRongAnh, unsigned int beCaoAnh );

// ---- tên SVG
void tenAnhSVG( char *tenAnhGoc, char *tenAnhPNG, char *tenKenh );

// ---- chậm mực
void veChamMucGoc0_den( FILE *tep, ChamMuc *chamMuc );
void veChamMucGoc0_mau( FILE *tep, ChamMuc *chamMuc, unsigned int mau, float doDuc );

void veChamMucGocXoay_den( FILE *tep, ChamMuc *chamMuc, float cosGoc, float sinGoc );
void veChamMucGocXoay_mau( FILE *tep, ChamMuc *chamMuc, float cosGoc, float sinGoc, unsigned int mau, float doDuc );

// ---- tập tin SVG
void dauTepSVG( FILE *tep, float beRong, float beCao );
void ketThucTepSVG( FILE *tep );

// ---- vẽ chậm
void vongTron_den( FILE *tep, float trungTamX, float trungTamY, float banKinh );
void vongTron_mau( FILE *tep, float trungTamX, float trungTamY, float banKinh, unsigned int mau, float doDuc );

void vuong_den( FILE *tep, float trungTamX, float trungTamY, float nuaKho, float kho );
void vuong_mau( FILE *tep, float trungTamX, float trungTamY, float nuaKho, float kho, unsigned int mau, float doDuc );

void vuongXoay_den( FILE *tep, float x, float y, float nuaKhoCosGoc, float nuaKhoSinGoc );
void vuongXoay_mau( FILE *tep, float x, float y, float nuaKhoCosGoc, float nuaKhoSinGoc, unsigned int mau, float doDuc );

void vuongGocTron_den( FILE *tep, float x, float y, float nuaKho, float banKinh, float nuaCachThang );
void vuongGocTron_mau( FILE *tep, float x, float y, float nuaKho, float banKinh, float nuaCachThang, unsigned int mau, float doDuc );

void vuongGocTronXoay_den( FILE *tep, float x, float y, float nuaKho, float banKinh, float nuaCachThang, float cosGoc, float sinGoc );
void vuongGocTronXoay_mau( FILE *tep, float x, float y, float nuaKho, float banKinh, float nuaCachThang, float cosGoc, float sinGoc, unsigned int mau, float doDuc );


int main( int argc, char *argv[] ) {
   
   if( argc > 1 ) {
      
      unsigned int beRongAnh = 0;
      unsigned int beCaoAnh = 0;
      unsigned char canLatMau = 0;
      unsigned char loaiAnh = 0;
      unsigned char *anhRGB = docPNG( argv[1], &beRongAnh, &beCaoAnh, &canLatMau, &loaiAnh );
      if( anhRGB == NULL ) {
         printf( "Vấn đề mở tệp %s\n", argv[1] );
         exit(0);
      }
      printf( "kho anh %d x %d\n", beRongAnh, beCaoAnh );
      unsigned buoc = 4;
      if( loaiAnh == 2 )
         buoc = 3;
      
      // ---- đổi RGB sang CMYK
      float *anhCMYK = NULL;
      if( buoc == 3 )
         anhCMYK = bienDoiRGBSangCMYK( anhRGB, beRongAnh, beCaoAnh );
      else {
         printf( "Chưa hỗ trợ ảnh độ xám và RGBA\n" );
         exit(0);
      }
      
      free( anhRGB );

      // ---- mở tệp
      char tenTepSVG_C[255];
      char tenTepSVG_M[255];
      char tenTepSVG_Y[255];
      char tenTepSVG_K[255];
      char tenTepSVG_CMYK[255];
      
      tenAnhSVG( argv[1], tenTepSVG_C, "C" );
      tenAnhSVG( argv[1], tenTepSVG_M, "M" );
      tenAnhSVG( argv[1], tenTepSVG_Y, "Y" );
      tenAnhSVG( argv[1], tenTepSVG_K, "K" );
      tenAnhSVG( argv[1], tenTepSVG_CMYK, "CMYK" );

      FILE *tepC = fopen( tenTepSVG_C, "w" );
      FILE *tepM = fopen( tenTepSVG_M, "w" );
      FILE *tepY = fopen( tenTepSVG_Y, "w" );
      FILE *tepK = fopen( tenTepSVG_K, "w" );
      FILE *tepCMYK = fopen( tenTepSVG_CMYK, "w" );
      
      
      dauTepSVG( tepC, beRongAnh + (kLE << 1), beCaoAnh + (kLE << 1));
      dauTepSVG( tepM, beRongAnh + (kLE << 1), beCaoAnh + (kLE << 1));
      dauTepSVG( tepY, beRongAnh + (kLE << 1), beCaoAnh + (kLE << 1));
      dauTepSVG( tepK, beRongAnh + (kLE << 1), beCaoAnh + (kLE << 1));
      dauTepSVG( tepCMYK, beRongAnh + (kLE << 1), beCaoAnh + (kLE << 1));
      
      // ---- xuất các tệp chẻ
      quetAnhGoc0_CMYK_toDen( tepY, anhCMYK, beRongAnh, beCaoAnh, 4, Y, kKHO_MUC );
      quetAnhGocKhac_CMYK_toDen( tepC, anhCMYK, beRongAnh, beCaoAnh, 4, C, kKHO_MUC, 0.1308997f );
      quetAnhGocKhac_CMYK_toDen( tepM, anhCMYK, beRongAnh, beCaoAnh, 4, M, kKHO_MUC, 0.6544985f );
      quetAnhGocKhac_CMYK_toDen( tepK, anhCMYK, beRongAnh, beCaoAnh, 4, K, kKHO_MUC, 0.7853982f );
      
      // ---- CMYK
      quetAnhGocKhac_CMYK_toMau( tepCMYK, anhCMYK, beRongAnh, beCaoAnh, 4, K, kKHO_MUC, 0.7853982f, 0x00, 0.7f );
      quetAnhGocKhac_CMYK_toMau( tepCMYK, anhCMYK, beRongAnh, beCaoAnh, 4, C, kKHO_MUC, 0.1308997f, 0x00ffff, 0.7f );
      quetAnhGocKhac_CMYK_toMau( tepCMYK, anhCMYK, beRongAnh, beCaoAnh, 4, M, kKHO_MUC, 0.6544985f, 0xff00ff, 0.7f );
      quetAnhGoc0_CMYK_toMau( tepCMYK, anhCMYK, beRongAnh, beCaoAnh, 4, Y, kKHO_MUC, 0xffff00, 1.0f );
      
      // ---- luu kết thúc tệp
      ketThucTepSVG( tepC );
      ketThucTepSVG( tepM );
      ketThucTepSVG( tepY );
      ketThucTepSVG( tepK );
      ketThucTepSVG( tepCMYK );
      
      // ---- đóng tệp
      fclose( tepC );
      fclose( tepM );
      fclose( tepY );
      fclose( tepK );
      fclose( tepCMYK );
      
      free( anhCMYK );
   }
   
   else {
      printf( "Gõ tên tệp xuất\n" );
   }
   return 1;
}

#pragma mark ---- ẢNH QUÉT GÓC 0 - LẬT ẢNH
void quetAnhGoc0_CMYK_toDen( FILE *tepSVG, float *anh, unsigned int beRongAnh, unsigned int beCaoAnh, unsigned char buoc, unsigned char soKenh, float khoMuc ) {
   
   ChamMuc chamMuc;
   chamMuc.kho = khoMuc;
   
   // ---- góc cho quét ảnh (đơn vị điểm ảnh đang quét)
   float gocQuetAnhX = 0.0f;
   float gocQuetAnhY = 0.0f;
   
   // ---- góc chấm mực (đơn vị SVG)
   float gocChamMucX = 0.0;
   float gocChamMucY = (float)beCaoAnh;
   gocChamMucX += kLE;
   gocChamMucY += kLE;
   
   // ---- tính số cột và hàng quét theo phân giải
   unsigned int soCotQuet = ceilf(beRongAnh/khoMuc);
   unsigned int soHangQuet = ceilf(beCaoAnh/khoMuc);
 //  printf( "Anh quét: soCot %d  soHang %d\n", soCotQuet, soHangQuet );
   
   // ---- quét ảnh
   unsigned short soHang = 0;
   
   while( soHang < soHangQuet ) {
      
      // ---- khởi động quét ngang
      float diemAnhX = gocQuetAnhX;
      float diemAnhY = gocQuetAnhY;
      
      // ---- vị trí chấm vào SVG
      chamMuc.x = gocChamMucX;
      chamMuc.y = gocChamMucY;
      
      unsigned short soCot = 0;
      while( soCot < soCotQuet ) {
         // ---- xem nếu vị trí đang  trong ảnh
         int viTriX = floorf(diemAnhX);
         int viTriY = floorf(diemAnhY);
         
         if( (viTriX < beRongAnh) && (viTriY < beCaoAnh) ) {
            unsigned int chiSoAnh = viTriX + viTriY*beRongAnh;
            chiSoAnh <<= 2;
            chamMuc.dienTich = anh[chiSoAnh + soKenh];
//            printf( "kenh %d %5.3f  (%d; %d)\n", soKenh, chamMuc.dienTich, viTriX, viTriY );
            veChamMucGoc0_den( tepSVG, &chamMuc );
         }
         // ---- nâng cấp vị trí mặc dụ có chấm mực hay không
         diemAnhX += khoMuc;
         chamMuc.x += khoMuc;
         soCot++;
      }
      
      gocChamMucY -= khoMuc;
      gocQuetAnhY += khoMuc;
      soHang++;
   }
}

void quetAnhGoc0_CMYK_toMau( FILE *tepSVG, float *anh, unsigned int beRongAnh, unsigned int beCaoAnh, unsigned char buoc, unsigned char soKenh, float khoMuc, unsigned int mau, float doDuc ) {
   
   ChamMuc chamMuc;
   chamMuc.kho = khoMuc;
   
   // ---- góc cho quét ảnh (đơn vị điểm ảnh đang quét)
   float gocQuetAnhX = 0.0f;
   float gocQuetAnhY = 0.0f;
   
   // ---- góc chấm mực (đơn vị SVG)
   float gocChamMucX = 0.0;
   float gocChamMucY = (float)beCaoAnh;
   gocChamMucX += kLE;
   gocChamMucY += kLE;
   
   // ---- tính số cột và hàng quét theo phân giải
   unsigned int soCotQuet = ceilf(beRongAnh/khoMuc);
   unsigned int soHangQuet = ceilf(beCaoAnh/khoMuc);
   printf( "Anh quét: soCot %d  soHang %d\n", soCotQuet, soHangQuet );
   
   // ---- quét ảnh
   unsigned short soHang = 0;
   
   while( soHang < soHangQuet ) {
      
      // ---- khởi động quét ngang
      float diemAnhX = gocQuetAnhX;
      float diemAnhY = gocQuetAnhY;
      
      // ---- vị trí chấm vào SVG
      chamMuc.x = gocChamMucX;
      chamMuc.y = gocChamMucY;
      
      unsigned short soCot = 0;
      while( soCot < soCotQuet ) {
         // ---- xem nếu vị trí đang  trong ảnh
         int viTriX = floorf(diemAnhX);
         int viTriY = floorf(diemAnhY);

         if( (viTriX < beRongAnh) && (viTriY < beCaoAnh) ) {
            unsigned int chiSoAnh = viTriX + viTriY*beRongAnh;
            chiSoAnh <<= 2;
            chamMuc.dienTich = anh[chiSoAnh + soKenh];
//            printf( "kenh %d %5.3f  (%d; %d)\n", soKenh, chamMuc.dienTich, viTriX, viTriY );
            veChamMucGoc0_mau( tepSVG, &chamMuc, mau, doDuc );
         }
         // ---- nâng cấp vị trí mặc dụ có chấm mực hay không
         diemAnhX += khoMuc;
         chamMuc.x += khoMuc;
         soCot++;
      }
      
      gocChamMucY -= khoMuc;
      gocQuetAnhY += khoMuc;
      soHang++;
   }
}

#pragma mark ---- ẢNH QUÉT GÓC TỰ DO - LẬT ẢNH
void quetAnhGocKhac_CMYK_toDen( FILE *tepSVG, float *anh, unsigned int beRongAnh, unsigned int beCaoAnh, unsigned char buoc, unsigned char soKenh, float khoMuc, float goc ) {
   
   // ---- quét ảnh
   ChamMuc chamMuc;
   chamMuc.kho = khoMuc;
   
   // ---- hạn chế góc; 0° ≤ góc ≤ 90°
   goc = fmodf( goc, kPI_CHIA_2 );
//   printf( "  goc: %5.3f\n", goc );
   
   // ---- chỉ cần tính một lần
   float sinGoc = sinf( goc );
   float cosGoc = cosf( goc );
   
   // ---- hai vectơ hướng chấm mực (đơn vị SVG)
   float cachNgangX = khoMuc*cosGoc;
   float cachNgangY = -khoMuc*sinGoc;
   float cachDocX = cachNgangY;
   float cachDocY = -cachNgangX;
   
   // ---- góc cho quét ảnh (đơn vị điển ảnh gốc)
   float gocQuetAnhX = (float)beRongAnh*sinGoc*sinGoc;
   float gocQuetAnhY = -(float)beRongAnh*sinGoc*cosGoc;
   
   // ---- vị trí chấm mực (phải xoay nghịch hướng, đơn vị SVG)
   float gocChamMucX = gocQuetAnhX;
   float gocChamMucY = (beCaoAnh - gocQuetAnhY);
   gocChamMucX += kLE;
   gocChamMucY += kLE;
   
//   printf( "gocQuet %5.3f %5.3f  gocChamMuc %5.3f %5.3f\n", gocQuetAnhX, gocQuetAnhY, gocChamMucX, gocChamMucY );
   
   // ---- tính số cột và hàng quét theo phân giải
   unsigned int soCotQuet = ceilf((beCaoAnh*sinGoc + beRongAnh*cosGoc)/khoMuc);
   unsigned int soHangQuet = ceilf((beCaoAnh*cosGoc + beRongAnh*sinGoc)/khoMuc);
//   printf( "Anh quét: soCotQuet %d  soHangQuet %d\n", soCotQuet, soHangQuet);
   
   unsigned short soHang = 0;
   
   while( soHang < soHangQuet ) {
      // ---- khởi động quét ngang
      float diemAnhX = gocQuetAnhX;
      float diemAnhY = gocQuetAnhY;
      
      // ---- vị trí chấm mực vào SVG
      chamMuc.x = gocChamMucX;
      chamMuc.y = gocChamMucY;
      
      unsigned short soCot = 0;
      while( soCot < soCotQuet ) {
         // ---- xem nếu vị trí đang  trong ảnh
         int viTriX = floorf(diemAnhX);
         int viTriY = floorf(diemAnhY);
         
         if( (viTriX >= 0) && (viTriY >= 0) ) {
            if( (viTriX < beRongAnh) && (viTriY < beCaoAnh) ) {
               unsigned int chiSoAnh = (viTriX + viTriY*beRongAnh);
               chiSoAnh *= buoc;
               // ---- điện tích bằng đồ sáng của điểm ảnh
               chamMuc.dienTich = anh[chiSoAnh+soKenh];
               // ---- chấm mực
               veChamMucGocXoay_den( tepSVG, &chamMuc, cosGoc, -sinGoc );
            }
         }
         // ---- nâng cấp vị trí mặc dụ có chấm mực hay không
         chamMuc.x += cachNgangX;
         chamMuc.y += cachNgangY;
         diemAnhX += cosGoc*khoMuc;
         diemAnhY += sinGoc*khoMuc;
         
         soCot++;
      }
      gocChamMucX += cachDocX;
      gocChamMucY += cachDocY;
      gocQuetAnhX -= sinGoc*khoMuc;
      gocQuetAnhY += cosGoc*khoMuc;
      soHang++;
   }
}

void quetAnhGocKhac_CMYK_toMau( FILE *tepSVG, float *anh, unsigned int beRongAnh, unsigned int beCaoAnh, unsigned char buoc, unsigned char soKenh, float khoMuc, float goc, unsigned int mau, float doDuc ) {
   
   // ---- quét ảnh
   ChamMuc chamMuc;
   chamMuc.kho = khoMuc;
   
   // ---- hạn chế góc; 0° ≤ góc ≤ 90°
   goc = fmodf( goc, kPI_CHIA_2 );
//   printf( "  goc: %5.3f\n", goc );
   
   // ---- chỉ cần tính một lần
   float sinGoc = sinf( goc );
   float cosGoc = cosf( goc );
   
   // ---- hai vectơ hướng chấm mực (đơn vị SVG)
   float cachNgangX = khoMuc*cosGoc;
   float cachNgangY = -khoMuc*sinGoc;
   float cachDocX = cachNgangY;
   float cachDocY = -cachNgangX;
   
   // ---- góc cho quét ảnh (đơn vị điển ảnh gốc)
   float gocQuetAnhX = (float)beRongAnh*sinGoc*sinGoc;
   float gocQuetAnhY = -(float)beRongAnh*sinGoc*cosGoc;
   
   // ---- vị trí chấm mực (phải xoay nghịch hướng, đơn vị SVG)
   float gocChamMucX = gocQuetAnhX;
   float gocChamMucY = (beCaoAnh - gocQuetAnhY);
   gocChamMucX += kLE;
   gocChamMucY += kLE;
   
//   printf( "gocQuet %5.3f %5.3f  gocChamMuc %5.3f %5.3f\n", gocQuetAnhX, gocQuetAnhY, gocChamMucX, gocChamMucY );
   
   // ---- tính số cột và hàng quét theo phân giải
   unsigned int soCotQuet = ceilf((beCaoAnh*sinGoc + beRongAnh*cosGoc)/khoMuc);
   unsigned int soHangQuet = ceilf((beCaoAnh*cosGoc + beRongAnh*sinGoc)/khoMuc);
//   printf( "Anh quét: soCotQuet %d  soHangQuet %d\n", soCotQuet, soHangQuet);
   
   unsigned short soHang = 0;
   
   while( soHang < soHangQuet ) {
      // ---- khởi động quét ngang
      float diemAnhX = gocQuetAnhX;
      float diemAnhY = gocQuetAnhY;
      
      // ---- vị trí chấm mực vào SVG
      chamMuc.x = gocChamMucX;
      chamMuc.y = gocChamMucY;
      
      unsigned short soCot = 0;
      while( soCot < soCotQuet ) {
         // ---- xem nếu vị trí đang  trong ảnh
         int viTriX = floorf(diemAnhX);
         int viTriY = floorf(diemAnhY);
         
         if( (viTriX >= 0) && (viTriY >= 0) ) {
            if( (viTriX < beRongAnh) && (viTriY < beCaoAnh) ) {
               unsigned int chiSoAnh = (viTriX + viTriY*beRongAnh);
               chiSoAnh *= buoc;
               // ---- điện tích bằng đồ sáng của điểm ảnh
               chamMuc.dienTich = anh[chiSoAnh+soKenh];
               // ---- chấm mực
               veChamMucGocXoay_mau( tepSVG, &chamMuc, cosGoc, -sinGoc, mau, doDuc );
            }
         }
         // ---- nâng cấp vị trí mặc dụ có chấm mực hay không
         chamMuc.x += cachNgangX;
         chamMuc.y += cachNgangY;
         diemAnhX += cosGoc*khoMuc;
         diemAnhY += sinGoc*khoMuc;
         
         soCot++;
      }
      gocChamMucX += cachDocX;
      gocChamMucY += cachDocY;
      gocQuetAnhX -= sinGoc*khoMuc;
      gocQuetAnhY += cosGoc*khoMuc;
      soHang++;
   }
}

#pragma mark ---- BIẾN ĐỔI RGB SANG CMYK
float *bienDoiRGBSangCMYK( unsigned char *anhRGB, unsigned int beRongAnh, unsigned int beCaoAnh ) {
   
   unsigned int chiSoCuoi = beRongAnh*beCaoAnh << 2;
   
   float *anhCMYK = malloc( chiSoCuoi*sizeof(float) );
   if( anhCMYK == NULL ) {
      printf( "Có vấn đề giành trí nhớ cho biên đổi sang CMYK\n" );
      exit(0);
   }
   
   unsigned int chiSoRGB = 0;
   unsigned int chiSoCMYK = 0;
   while( chiSoCMYK < chiSoCuoi ) {
      // ---- đơn vị hóa trước
      float mauDo = (float)anhRGB[chiSoRGB]/255.0f;
      float mauLuc = (float)anhRGB[chiSoRGB+1]/255.0f;
      float mauXanh = (float)anhRGB[chiSoRGB+2]/255.0f;
//      printf( "RGB %5.3f %5.3f %5.3f\n", mauDo, mauLuc, mauXanh );
      
      float lonNhat = mauDo;
      if( mauLuc > lonNhat )
         lonNhat = mauLuc;
      if( mauXanh > lonNhat )
         lonNhat = mauXanh;
      
      float mauDen = 1.0f - lonNhat;
      float mauXanhDaTroi = 0.0f;
      float mauTim = 0.0f;
      float mauVang = 0.0f;
      
      if( mauDen < 1.0f ) {
         float mauSo = 1.0f - mauDen;
         mauXanhDaTroi = (mauSo - mauDo)/mauSo;
         mauTim = (mauSo - mauLuc)/mauSo;
         mauVang = (mauSo - mauXanh)/mauSo;
      }
      // ----
      anhCMYK[chiSoCMYK] = mauXanhDaTroi;
      anhCMYK[chiSoCMYK+M] = mauTim;
      anhCMYK[chiSoCMYK+Y] = mauVang;
      anhCMYK[chiSoCMYK+K] = mauDen;
//      printf( "CMYK %5.3f %5.3f %5.3f %5.3f\n", mauXanhDaTroi, mauTim, mauVang, mauDen );

      
      chiSoRGB += 3;
      chiSoCMYK += 4;
   }

   return anhCMYK;
}

#pragma mark ---- TÊN ẢNH
void tenAnhSVG( char *tenAnhGoc, char *tenAnhPNG, char *tenKenh ) {
   
   // ---- chép tên ảnh gốc
   while( (*tenAnhGoc != 0x00) && (*tenAnhGoc != '.') ) {
      *tenAnhPNG = *tenAnhGoc;
      tenAnhPNG++;
      tenAnhGoc++;
   }
   
   // ----
   unsigned char beDaiTenKenh = 0;
   char *xauTenKenh = tenKenh;
   while( *xauTenKenh != 0x00 ) {
      xauTenKenh++;
      beDaiTenKenh++;
   }
   
   // ---- kèm tên kênh
   *tenAnhPNG = '_';
   tenAnhPNG++;
   
   unsigned char chiSoKyTu = 0;
   while( chiSoKyTu < beDaiTenKenh ) {
      *tenAnhPNG = *tenKenh;
      tenAnhPNG++;
      tenKenh++;
      chiSoKyTu++;
   }
   
   // ---- kèm đuôi SVG
   *tenAnhPNG = '.';
   tenAnhPNG++;
   *tenAnhPNG = 's';
   tenAnhPNG++;
   *tenAnhPNG = 'v';
   tenAnhPNG++;
   *tenAnhPNG = 'g';
   tenAnhPNG++;
   *tenAnhPNG = 0x0;
   tenAnhPNG++;
}



#pragma mark ---- VẼ CHẤM
// Cho diện tích 0 ≤ D ≤ π/4
// +----******----+     b - bán kính = √(D/π)
// |  **      **  |     D - diện tích = πb^2
// |*    ****    *|
// *   *      *   *
// *   *      *   *
// |*    ****    *|
// |  **      **  |
// +----******----+
//
// Cho diện tích π/4 < D ≤ 1
// +---*******+---+      b - bán kính
// | **|      |** | b    D - diện tích
// *---+      +---*      c - cách thẳng = 1 - 2b
// *              * c    b = √[(1 - D)/(4 - π)]
// *              *
// *---+      +---*
// | **|      |** | b
// +---*******+---+
//
// |< c >|<- 2b ->|
// +-----+---**---+      b - bán kính
// |     |**    **|      D - diện tích = 1 + πb^2 - 4b^2
// |     *        *      c - cách thẳng = 1 - 2b
// |     *        *      b = √[(1 - D)/(4 - π)]
// |     |**    **|
// |     +---**---+
// |              |
// +--------------+


void veChamMucGoc0_den( FILE *tep, ChamMuc *chamMuc ) {
   
   float kho = chamMuc->kho;
   float nuaKho = kho*0.5f;
   float x = chamMuc->x;
   float y = chamMuc->y;
   float dienTich = chamMuc->dienTich;
   unsigned short soLuongKyTu = 0;
   
   // ---- phân loại
   if( dienTich >= 1.0f ) {
      // ---- vẽ vuông
      vuong_den( tep, x, y, nuaKho, kho );
   }
   else if( dienTich > kPI_CHIA_4 ) {
      // ---- vẽ vuông góc giác tròn
      float banKinh = sqrtf( (1 - dienTich)/k4_TRU_PI );
      float cachThang = 1.0f - 2.0f*banKinh;
      
      if( cachThang < 0.0 )
         // ---- vẽ vòng tròn
         vongTron_den( tep, x, y, banKinh*nuaKho );
      else
         // ---- vẽ vuông góc giác tròn
         vuongGocTron_den( tep, x, y, 0.5f*kho, banKinh*kho, cachThang*kho*0.5f );
   }
   else if( dienTich > 0.0f ) {
      float banKinh = sqrtf( dienTich/kPI_CHIA_4 );
      // ---- vẽ vòng tròn
      vongTron_den( tep, x, y, banKinh*nuaKho );
   }
   
}

void veChamMucGoc0_mau( FILE *tep, ChamMuc *chamMuc, unsigned int mau, float doDuc ) {
   
   float kho = chamMuc->kho;
   float nuaKho = kho*0.5f;
   float x = chamMuc->x;
   float y = chamMuc->y;
   float dienTich = chamMuc->dienTich;
   unsigned short soLuongKyTu = 0;
   
   // ---- phân loại
   if( dienTich >= 1.0f ) {
      // ---- vẽ vuông
      vuong_mau( tep, x, y, nuaKho, kho, mau, doDuc );
   }
   else if( dienTich > kPI_CHIA_4 ) {
      // ---- vẽ vuông góc giác tròn
      float banKinh = sqrtf( (1 - dienTich)/k4_TRU_PI );
      float cachThang = 1.0f - 2.0f*banKinh;
      
      if( cachThang < 0.0 )
         // ---- vẽ vòng tròn
         vongTron_mau( tep, x, y, banKinh*nuaKho, mau, doDuc );
      else
         // ---- vẽ vuông góc giác tròn
         vuongGocTron_mau( tep, x, y, 0.5f*kho, banKinh*kho, cachThang*kho*0.5f, mau, doDuc );
   }
   else if( dienTich > 0.0f ) {
      float banKinh = sqrtf( dienTich/kPI_CHIA_4 );
      // ---- vẽ vòng tròn
      vongTron_mau( tep, x, y, banKinh*nuaKho, mau, doDuc );
   }
   
}

void veChamMucGocXoay_den( FILE *tep, ChamMuc *chamMuc, float cosGoc, float sinGoc ) {
   
   float kho = chamMuc->kho;
   float nuaKho = kho*0.5f;
   float x = chamMuc->x;
   float y = chamMuc->y;
   float dienTich = chamMuc->dienTich;
   unsigned short soLuongKyTu = 0;
   
   // ---- phân loại
   if( dienTich >= 1.0f ) {
      // ---- vẽ vuông
      vuongXoay_den( tep, x, y, nuaKho*cosGoc, nuaKho*sinGoc );
   }
   else if( dienTich > kPI_CHIA_4 ) {
      // ---- vẽ vuông góc giác tròn
      float banKinh = sqrtf( (1 - dienTich)/k4_TRU_PI );
      float cachThang = 1.0f - 2.0f*banKinh;
      
      if( cachThang < 0.0f )
         // ---- vẽ vòng tròn
         vongTron_den( tep, x, y, banKinh*nuaKho );
      else
         // ---- vẽ vuông góc giác tròn
         vuongGocTronXoay_den( tep, x, y, 0.5f*kho, banKinh*kho, cachThang*kho*0.5f, cosGoc, sinGoc );
   }
   else if( dienTich > 0.0f ) {
      float banKinh = sqrtf( dienTich/kPI_CHIA_4 );
      // ---- vẽ vòng tròn
      vongTron_den( tep, x, y, banKinh*nuaKho );
   }
}

void veChamMucGocXoay_mau( FILE *tep, ChamMuc *chamMuc, float cosGoc, float sinGoc, unsigned int mau, float doDuc ) {
   
   float kho = chamMuc->kho;
   float nuaKho = kho*0.5f;
   float x = chamMuc->x;
   float y = chamMuc->y;
   float dienTich = chamMuc->dienTich;
   unsigned short soLuongKyTu = 0;
   
   // ---- phân loại
   if( dienTich >= 1.0f ) {
      // ---- vẽ vuông
      vuongXoay_mau( tep, x, y, nuaKho*cosGoc, nuaKho*sinGoc, mau, doDuc );
   }
   else if( dienTich > kPI_CHIA_4 ) {
      // ---- vẽ vuông góc giác tròn
      float banKinh = sqrtf( (1 - dienTich)/k4_TRU_PI );
      float cachThang = 1.0f - 2.0f*banKinh;
      
      if( cachThang < 0.0f )
         // ---- vẽ vòng tròn
         vongTron_mau( tep, x, y, banKinh*nuaKho, mau, doDuc );
      else
         // ---- vẽ vuông góc giác tròn
         vuongGocTronXoay_mau( tep, x, y, 0.5f*kho, banKinh*kho, cachThang*kho*0.5f, cosGoc, sinGoc, mau, doDuc );
   }
   else if( dienTich > 0.0f ) {
      float banKinh = sqrtf( dienTich/kPI_CHIA_4 );
      // ---- vẽ vòng tròn
      vongTron_mau( tep, x, y, banKinh*nuaKho, mau, doDuc );
   }
}

#pragma mark ---- SVG
void dauTepSVG( FILE *tep, float beRong, float beCao ) {
   fprintf( tep, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n" );
   fprintf( tep, "<!-- Chế tạo bằng Chấm Mục -->\n\n" );
   fprintf( tep, "<svg version=\"1.1\"  width=\"%.0f\" height=\"%.0f\" >\n\n", beRong, beCao );
}

void ketThucTepSVG( FILE *tep ) {
   fprintf( tep, "</svg>" );
}


#pragma mark ---- Vẽ Chấm
void vongTron_den( FILE *tep, float trungTamX, float trungTamY, float banKinh ) {
   
   fprintf( tep, "<circle cx=\"%5.4f\" cy=\"%5.4f\" r=\"%5.4f\" ",
           trungTamX, trungTamY, banKinh );
   fprintf( tep, "fill=\"#000000\" fill-opacity=\"1.0\" stroke=\"none\" />\n" );
}

void vongTron_mau( FILE *tep, float trungTamX, float trungTamY, float banKinh, unsigned int mau, float doDuc ) {
   
   fprintf( tep, "<circle cx=\"%5.4f\" cy=\"%5.4f\" r=\"%5.4f\" ",
           trungTamX, trungTamY, banKinh );
   fprintf( tep, "fill=\"#%06x\" fill-opacity=\"%5.5f\" stroke=\"none\" />\n", mau, doDuc );
}

// -----
void vuong_den( FILE *tep, float x, float y, float nuaKho, float kho ) {
   
   fprintf( tep, "<rect x=\"%7.5f\" y=\"%7.5f\" width=\"%7.5f\" height=\"%7.5f\" ", x - nuaKho, y - nuaKho, kho, kho );
   
   fprintf( tep, "fill=\"#000000\" fill-opacity=\"1.0\" stroke=\"none\" />\n" );
}

void vuong_mau( FILE *tep, float x, float y, float nuaKho, float kho, unsigned int mau, float doDuc ) {
   
   fprintf( tep, "<rect x=\"%7.5f\" y=\"%7.5f\" width=\"%7.5f\" height=\"%7.5f\" ", x - nuaKho, y - nuaKho, kho, kho );
   
   fprintf( tep, "fill=\"#%06x\" fill-opacity=\"%5.5f\" stroke=\"none\" />\n", mau, doDuc );
}

// -----
void vuongXoay_den( FILE *tep, float x, float y, float nuaKhoCosGoc, float nuaKhoSinGoc ) {
   
   fprintf( tep, "<polygon points=\"%7.5f, %7.5f  %7.5f, %7.5f  %7.5f, %7.5f  %7.5f, %7.5f\" ",
           x - nuaKhoCosGoc + nuaKhoSinGoc, y - nuaKhoCosGoc - nuaKhoSinGoc,
           x + nuaKhoCosGoc + nuaKhoSinGoc, y - nuaKhoCosGoc + nuaKhoSinGoc,
           x + nuaKhoCosGoc - nuaKhoSinGoc, y + nuaKhoCosGoc + nuaKhoSinGoc,
           x - nuaKhoCosGoc - nuaKhoSinGoc, y + nuaKhoCosGoc - nuaKhoSinGoc);
   
   fprintf( tep, "fill=\"#000000\" fill-opacity=\"1.0\" stroke=\"none\" />\n" );
}

void vuongXoay_mau( FILE *tep, float x, float y, float nuaKhoCosGoc, float nuaKhoSinGoc, unsigned int mau, float doDuc ) {
   
   fprintf( tep, "<polygon points=\"%7.5f, %7.5f  %7.5f, %7.5f  %7.5f, %7.5f  %7.5f, %7.5f\" ",
           x - nuaKhoCosGoc + nuaKhoSinGoc, y - nuaKhoCosGoc - nuaKhoSinGoc,
           x + nuaKhoCosGoc + nuaKhoSinGoc, y - nuaKhoCosGoc + nuaKhoSinGoc,
           x + nuaKhoCosGoc - nuaKhoSinGoc, y + nuaKhoCosGoc + nuaKhoSinGoc,
           x - nuaKhoCosGoc - nuaKhoSinGoc, y + nuaKhoCosGoc - nuaKhoSinGoc);
   
   fprintf( tep, "fill=\"#%06x\" fill-opacity=\"%5.5f\" stroke=\"none\" />\n", mau, doDuc );
}


// Bezier  cho vòng tròn =  0,558*bánKính

// Thứ tự điểm
//       16  14/15        12/13  11
//           --+-----------+--
//  17    -                    - 10       ^ +y
// 18/19 +                      + 8/9     |
//       |                      |         |
//       |           +          |         +-----> +x
//       |                      |
// 20/21 +                      + 6/7
//  22    -                    -  5
//           --+-----------+--
//       23   0/1        2/3  4

void vuongGocTron_den( FILE *tep, float x, float y, float nuaKho, float banKinh, float nuaCachThang ) {
   
   // ---- tính vị trí cho điểm điều khiển Bezier
   float toaDoDiemDieuKhien = nuaCachThang + banKinh*0.558f;
   
   // ---- điểm khởi đầu
   fprintf( tep, "<path d=\"M %7.5f, %7.5f ",
           -nuaCachThang + x, -nuaKho + y );
   // ---- khúc thhẳng dưới
   fprintf( tep, "\nL %7.5f,%7.5f",
           nuaCachThang + x, -nuaKho + y );
   // ---- góc phải dưới
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           toaDoDiemDieuKhien + x, -nuaKho + y,
           nuaKho + x, -toaDoDiemDieuKhien + y,
           nuaKho + x, -nuaCachThang + y );
   // ---- khúc thhẳng phải
   fprintf( tep, "\nL %7.5f,%7.5f",
           nuaKho + x, nuaCachThang + y );
   // ---- góc phải tren
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           nuaKho + x, toaDoDiemDieuKhien + y,
           toaDoDiemDieuKhien + x, nuaKho + y,
           nuaCachThang + x, nuaKho + y );
   // ---- khúc thhẳng trên
   fprintf( tep, "\nL %7.5f,%7.5f",
           -nuaCachThang + x, nuaKho + y );
   // ---- góc trái tren
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           -toaDoDiemDieuKhien + x, nuaKho + y,
           -nuaKho + x, toaDoDiemDieuKhien + y,
           -nuaKho + x, nuaCachThang + y );
   // ---- khúc thhẳng trái
   fprintf( tep, "\nL %7.5f,%7.5f",
           -nuaKho + x, -nuaCachThang + y );
   // ---- góc trái dưới
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f Z\" \n",
           -nuaKho + x, -toaDoDiemDieuKhien + y,
           -toaDoDiemDieuKhien + x, -nuaKho + y,
           -nuaCachThang + x, -nuaKho + y );
   
   // ---- kết thúc
   fprintf( tep, "fill=\"#000000\" fill-opacity=\"1.0\" stroke=\"none\" />\n" );
}

void vuongGocTron_mau( FILE *tep, float x, float y, float nuaKho, float banKinh, float nuaCachThang, unsigned int mau, float doDuc ) {
   
   // ---- tính vị trí cho điểm điều khiển Bezier
   float toaDoDiemDieuKhien = nuaCachThang + banKinh*0.558f;
   
   // ---- điểm khởi đầu
   fprintf( tep, "<path d=\"M %7.5f, %7.5f ",
           -nuaCachThang + x, -nuaKho + y );
   // ---- khúc thhẳng dưới
   fprintf( tep, "\nL %7.5f,%7.5f",
           nuaCachThang + x, -nuaKho + y );
   // ---- góc phải dưới
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           toaDoDiemDieuKhien + x, -nuaKho + y,
           nuaKho + x, -toaDoDiemDieuKhien + y,
           nuaKho + x, -nuaCachThang + y );
   // ---- khúc thhẳng phải
   fprintf( tep, "\nL %7.5f,%7.5f",
           nuaKho + x, nuaCachThang + y );
   // ---- góc phải tren
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           nuaKho + x, toaDoDiemDieuKhien + y,
           toaDoDiemDieuKhien + x, nuaKho + y,
           nuaCachThang + x, nuaKho + y );
   // ---- khúc thhẳng trên
   fprintf( tep, "\nL %7.5f,%7.5f",
           -nuaCachThang + x, nuaKho + y );
   // ---- góc trái tren
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           -toaDoDiemDieuKhien + x, nuaKho + y,
           -nuaKho + x, toaDoDiemDieuKhien + y,
           -nuaKho + x, nuaCachThang + y );
   // ---- khúc thhẳng trái
   fprintf( tep, "\nL %7.5f,%7.5f",
           -nuaKho + x, -nuaCachThang + y );
   // ---- góc trái dưới
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f Z\" \n",
           -nuaKho + x, -toaDoDiemDieuKhien + y,
           -toaDoDiemDieuKhien + x, -nuaKho + y,
           -nuaCachThang + x, -nuaKho + y );
   
   // ---- kết thúc
   fprintf( tep, "fill=\"#%06x\" fill-opacity=\"%5.5f\" stroke=\"none\" />\n", mau, doDuc );
}

// Thứ tự điểm
//       16  14/15        12/13  11
//           --+-----------+--
//  17    -                     - 10       ^ +y
// 18/19 +                       + 8/9     |
//       |                       |         |
//       |           +           |         +-----> +x
//       |                       |
// 20/21 +                       + 6/7
//  22    -                     -  5
//           --+-----------+--
//       23   0/1        2/3  4
//       |<--->|<--------->|<--->|
//              cáchThẳng   bánKính

void vuongGocTronXoay_den( FILE *tep, float x, float y, float nuaKho, float banKinh, float nuaCachThang, float cosGoc, float sinGoc ) {
   
   // ---- tính vị trí cho điểm điều khiển Bezier
   
   float nuaCachThangCos = nuaCachThang*cosGoc;
   float nuaCachThangSin = nuaCachThang*sinGoc;
   
   float nuaKhoCos = nuaKho*cosGoc;
   float nuaKhoSin = nuaKho*sinGoc;
   
   float dieuKhien = nuaCachThang + banKinh*0.558f;
   float dieuKhienCos = dieuKhien*cosGoc;
   float dieuKhienSin = dieuKhien*sinGoc;
   
   // ---- điểm khởi đầu - 0
   fprintf( tep, "<path d=\"M %7.5f, %7.5f ",
           -nuaCachThangCos + nuaKhoSin + x, -nuaKhoCos - nuaCachThangSin + y);
   // ---- khúc thhẳng dưới - 1; 2; 3
   fprintf( tep, "\nL %7.5f,%7.5f",
           nuaCachThangCos + nuaKhoSin + x, -nuaKhoCos + nuaCachThangSin + y);
   // ---- góc phải dưới - 4; 5; 6
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           dieuKhienCos + nuaKhoSin + x, -nuaKhoCos + dieuKhienSin + y,
           nuaKhoCos + dieuKhienSin + x, -dieuKhienCos + nuaKhoSin + y,
           nuaKhoCos + nuaCachThangSin + x, -nuaCachThangCos + nuaKhoSin + y);
   // ---- khúc thhẳng phải - 7; 8; 9
   fprintf( tep, "\nL %7.5f,%7.5f",
           nuaKhoCos - nuaCachThangSin + x, nuaCachThangCos + nuaKhoSin + y);
   // ---- góc phải tren - 10; 11; 12
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           nuaKhoCos - dieuKhienSin + x, dieuKhienCos + nuaKhoSin + y,
           dieuKhienCos - nuaKhoSin + x, nuaKhoCos + dieuKhienSin + y,
           nuaCachThangCos - nuaKhoSin + x, nuaKhoCos + nuaCachThangSin + y);
   // ---- khúc thhẳng trên - 13; 14; 15
   fprintf( tep, "\nL %7.5f,%7.5f",
           -nuaCachThangCos - nuaKhoSin + x, nuaKhoCos - nuaCachThangSin + y);
   // ---- góc trái tren - 16; 17; 18
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           -dieuKhien - nuaKhoSin + x, nuaKhoCos - dieuKhienSin + y,
           -nuaKhoCos - dieuKhienSin + x, dieuKhienCos - nuaKhoSin + y,
           -nuaKhoCos - nuaCachThangSin + x, nuaCachThang - nuaKhoSin + y);
   // ---- khúc thhẳng trái - 19; 20; 21
   fprintf( tep, "\nL %7.5f,%7.5f",
           -nuaKhoCos + nuaCachThangSin + x, -nuaCachThangCos - nuaKhoSin + y );
   // ---- góc trái dưới - 22; 23; 0
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f Z\"\n",
           -nuaKhoCos + dieuKhienSin + x, -dieuKhienCos - nuaKhoSin + y,
           -dieuKhienCos + nuaKhoSin + x, -nuaKhoCos - dieuKhienSin + y,
           -nuaCachThangCos + nuaKhoSin + x, -nuaKhoCos - nuaCachThangSin + y);
   
   // ---- kết thúc
   fprintf( tep, " fill=\"#000000\" fill-opacity=\"1.0\" stroke=\"none\" />\n" );
}

void vuongGocTronXoay_mau( FILE *tep, float x, float y, float nuaKho, float banKinh, float nuaCachThang, float cosGoc, float sinGoc, unsigned int mau, float doDuc ) {
   
   // ---- tính vị trí cho điểm điều khiển Bezier
   
   float nuaCachThangCos = nuaCachThang*cosGoc;
   float nuaCachThangSin = nuaCachThang*sinGoc;
   
   float nuaKhoCos = nuaKho*cosGoc;
   float nuaKhoSin = nuaKho*sinGoc;
   
   float dieuKhien = nuaCachThang + banKinh*0.558f;
   float dieuKhienCos = dieuKhien*cosGoc;
   float dieuKhienSin = dieuKhien*sinGoc;
   
   // ---- điểm khởi đầu - 0
   fprintf( tep, "<path d=\"M %7.5f, %7.5f ",
           -nuaCachThangCos + nuaKhoSin + x, -nuaKhoCos - nuaCachThangSin + y);
   // ---- khúc thhẳng dưới - 1; 2; 3
   fprintf( tep, "\nL %7.5f,%7.5f",
           nuaCachThangCos + nuaKhoSin + x, -nuaKhoCos + nuaCachThangSin + y);
   // ---- góc phải dưới - 4; 5; 6
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           dieuKhienCos + nuaKhoSin + x, -nuaKhoCos + dieuKhienSin + y,
           nuaKhoCos + dieuKhienSin + x, -dieuKhienCos + nuaKhoSin + y,
           nuaKhoCos + nuaCachThangSin + x, -nuaCachThangCos + nuaKhoSin + y);
   // ---- khúc thhẳng phải - 7; 8; 9
   fprintf( tep, "\nL %7.5f,%7.5f",
           nuaKhoCos - nuaCachThangSin + x, nuaCachThangCos + nuaKhoSin + y);
   // ---- góc phải tren - 10; 11; 12
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           nuaKhoCos - dieuKhienSin + x, dieuKhienCos + nuaKhoSin + y,
           dieuKhienCos - nuaKhoSin + x, nuaKhoCos + dieuKhienSin + y,
           nuaCachThangCos - nuaKhoSin + x, nuaKhoCos + nuaCachThangSin + y);
   // ---- khúc thhẳng trên - 13; 14; 15
   fprintf( tep, "\nL %7.5f,%7.5f",
           -nuaCachThangCos - nuaKhoSin + x, nuaKhoCos - nuaCachThangSin + y);
   // ---- góc trái tren - 16; 17; 18
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f",
           -dieuKhien - nuaKhoSin + x, nuaKhoCos - dieuKhienSin + y,
           -nuaKhoCos - dieuKhienSin + x, dieuKhienCos - nuaKhoSin + y,
           -nuaKhoCos - nuaCachThangSin + x, nuaCachThang - nuaKhoSin + y);
   // ---- khúc thhẳng trái - 19; 20; 21
   fprintf( tep, "\nL %7.5f,%7.5f",
           -nuaKhoCos + nuaCachThangSin + x, -nuaCachThangCos - nuaKhoSin + y );
   // ---- góc trái dưới - 22; 23; 0
   fprintf( tep, "\nC %7.5f,%7.5f %7.5f,%7.5f %7.5f,%7.5f Z\"\n",
           -nuaKhoCos + dieuKhienSin + x, -dieuKhienCos - nuaKhoSin + y,
           -dieuKhienCos + nuaKhoSin + x, -nuaKhoCos - dieuKhienSin + y,
           -nuaCachThangCos + nuaKhoSin + x, -nuaKhoCos - nuaCachThangSin + y);
   
   // ---- kết thúc
   fprintf( tep, "fill=\"#%06x\" fill-opacity=\"%5.5f\" stroke=\"none\" />\n", mau, doDuc );
}
