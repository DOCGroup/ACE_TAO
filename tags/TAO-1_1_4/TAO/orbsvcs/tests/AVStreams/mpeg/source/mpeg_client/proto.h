/* $Id$ */


#ifdef __STDC__
# define        P(s) s
#else
# define P(s) ()
#endif


/* util.c */
void correct_underflow P((void ));
int next_bits P((int num , unsigned int mask ));
char *get_ext_data P((void ));
int next_start_code P((void ));
char *get_extra_bit_info P((void ));

/* video.c */
void init_stats P((void ));
void PrintAllStats P((void ));
double ReadSysClock P((void ));
void PrintTimeInfo P((void ));
VidStream *NewVidStream P((void ));
void DestroyVidStream P((VidStream *astream ));
PictImage *NewPictImage P((unsigned int width , unsigned int height ));
void DestroyPictImage P((PictImage *apictimage ));
int mpegVidRsrc P((char *p ));
void ToggleBFlag P((void ));
void TogglePFlag P((void ));

/* parseblock.c */
void ParseReconBlock P((int n ));
void ParseAwayBlock P((int n ));

/* motionvector.c */
void ComputeForwVector P((int *recon_right_for_ptr , int *recon_down_for_ptr ));
void ComputeBackVector P((int *recon_right_back_ptr , int *recon_down_back_ptr ));

/* decoders.c */
void init_tables P((void ));
void decodeDCTDCSizeLum P((unsigned int *value ));
void decodeDCTDCSizeChrom P((unsigned int *value ));
void decodeDCTCoeffFirst P((unsigned int *run , int *level ));
void decodeDCTCoeffNext P((unsigned int *run , int *level ));

/* main.c */
int get_more_data P((unsigned int *buf_start , int max_length , int *length_ptr , unsigned int **buf_ptr ));
int main P((int argc , char **argv ));
void usage P((char *s ));

/* gdith.c */
void InitColor P((void ));
int HandleXError P((Display *dpy , XErrorEvent *event ));
void InstallXErrorHandler P((void ));
void DeInstallXErrorHandler P((void ));
void ResizeDisplay P((int w , int h ));
void InitDisplay P((char *name ));
void InitGrayDisplay P((char *name ));
void InitMonoDisplay P((char *name ));
void InitColorDisplay P((char *name ));
void ExecuteDisplay P((VidStream *vid_stream ));

/* fs2.c */
void InitFS2Dither P((void ));
void FS2DitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *disp , int rows , int cols ));

/* fs2fast.c */
void InitFS2FastDither P((void ));
void FS2FastDitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *out , int h , int w ));

/* fs4.c */
void InitFS4Dither P((void ));
void FS4DitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *disp , int rows , int cols ));

/* hybrid.c */
void InitHybridDither P((void ));
void HybridDitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *out , int h , int w ));

/* hybriderr.c */
void InitHybridErrorDither P((void ));
void HybridErrorDitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *out , int h , int w ));

/* 2x2.c */
void Init2x2Dither P((void ));
void RandInit P((int h , int w ));
void PostInit2x2Dither P((void ));
void Twox2DitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *out , int h , int w ));

/* gray.c */
void GrayDitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *out , int h , int w ));

/* mono.c */
void MonoDitherImage(register unsigned char *lum, unsigned char *cr,
                     unsigned char *cb, unsigned char *out,
                     int h, int w);
void MonoThresholdImage(unsigned char *lum, unsigned char *cr,
                        unsigned char *cb, unsigned char *out,
                        int h, int w);

/* jrevdct.c */
void init_pre_idct P((void ));
void j_rev_dct_sparse P((DCTBLOCK data , int pos ));
void j_rev_dct P((DCTBLOCK data ));
void j_rev_dct_sparse P((DCTBLOCK data , int pos ));
void j_rev_dct P((DCTBLOCK data ));

/* 24bit.c */
void InitColorDither P((void ));
void ColorDitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *out , int rows , int cols ));

/* util32.c */
Visual *FindFullColorVisual P((Display *dpy , int *depth ));
Window CreateFullColorWindow P((Display *dpy , int x , int y , int w , int h ));

/* ordered.c */
void InitOrderedDither P((void ));
void OrderedDitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *out , int h , int w ));

/* ordered2.c */
void InitOrdered2Dither P((void ));
void Ordered2DitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *out , int h , int w ));

/* mb_ordered.c */
void InitMBOrderedDither P((void ));
void MBOrderedDitherImage P((unsigned char *lum , unsigned char *cr , unsigned char *cb , unsigned char *out , int h , int w ));
void MBOrderedDitherDisplayCopy P((VidStream *vid_stream , int mb_addr , int motion_forw , int r_right_forw , int r_down_forw , int motion_back , int r_right_back , int r_down_back , unsigned char *past , unsigned char *future ));

#undef P
