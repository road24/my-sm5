/** @brief ActorMultiVertex - A texture created from multiple textures. */

#include "Actor.h"
#include "RageDisplay.h"
#include "RageTextureID.h"

enum DrawMode
{
	DrawMode_Quads = 0,
	DrawMode_QuadStrip,
	DrawMode_Fan,
	DrawMode_Strip,
	DrawMode_Triangles,
	DrawMode_LineStrip,
	DrawMode_SymmetricQuadStrip,
	NUM_DrawMode,
	DrawMode_Invalid
};

const RString& DrawModeToString( DrawMode cat );
const RString& DrawModeToLocalizedString( DrawMode cat );
LuaDeclareType( DrawMode );

class RageTexture;

class ActorMultiVertex: public Actor
{
public:
	ActorMultiVertex();
	ActorMultiVertex( const ActorMultiVertex &cpy );
	virtual ~ActorMultiVertex();

	void LoadFromNode( const XNode* Node );
	virtual ActorMultiVertex *Copy() const;

	struct AMV_TweenState
	{
		AMV_TweenState(): _DrawMode(DrawMode_Invalid), FirstToDraw(0), NumToDraw(-1), line_width(1.0f) {}
		static void MakeWeightedAverage(AMV_TweenState& average_out, const AMV_TweenState& ts1, const AMV_TweenState& ts2, float percent_between);
		bool operator==(const AMV_TweenState& other) const;
		bool operator!=(const AMV_TweenState& other) const { return !operator==(other); }

		void SetDrawState( DrawMode dm, int first, int num );
		int GetSafeNumToDraw( DrawMode dm, int num ) const;

		vector<RageSpriteVertex> vertices;

		DrawMode _DrawMode;
		int FirstToDraw;
		int NumToDraw;

		// needed for DrawMode_LineStrip
		float line_width;
	};

	AMV_TweenState& AMV_DestTweenState()
	{
		if(AMV_Tweens.empty())
		{ return AMV_current; }
		else
		{ return AMV_Tweens.back(); }
	}
	const AMV_TweenState& AMV_DestTweenState() const { return const_cast<ActorMultiVertex*>(this)->AMV_DestTweenState(); }

	virtual bool EarlyAbortDraw() const;
	virtual void DrawPrimitives();
	virtual void DrawInternal( const AMV_TweenState *TS );
	
	void SetCurrentTweenStart();
	void EraseHeadTween();
	void UpdatePercentThroughTween( float PercentThroughTween );
	void BeginTweening( float time, ITween *pInterp );

	void StopTweening();
	void FinishTweening();
	
	void SetTexture( RageTexture *Texture );
	RageTexture* GetTexture() { return _Texture; };
	void LoadFromTexture( RageTextureID ID );

	void UnloadTexture();
	void SetNumVertices( size_t n );

	void AddVertex();
	void AddVertices( int Add );

	void SetEffectMode( EffectMode em)			{ _EffectMode = em; }
	void SetTextureMode( TextureMode tm)		{ _TextureMode = tm; }
	void SetLineWidth( float width)				{ AMV_DestTweenState().line_width = width; }

	void SetDrawState( DrawMode dm, int first, int num ) { AMV_DestTweenState().SetDrawState(dm, first, num); }

	DrawMode GetDestDrawMode() const { return AMV_DestTweenState()._DrawMode; }
	int GetDestFirstToDraw() const					{ return AMV_DestTweenState().FirstToDraw; }
	int GetDestNumToDraw() const					{ return AMV_DestTweenState().NumToDraw; }
	DrawMode GetCurrDrawMode() const { return AMV_current._DrawMode; }
	int GetCurrFirstToDraw() const					{ return AMV_current.FirstToDraw; }
	int GetCurrNumToDraw() const					{ return AMV_current.NumToDraw; }
	size_t GetNumVertices() 					{ return AMV_DestTweenState().vertices.size(); }
	
	void SetVertexPos( int index , float x , float y , float z );
	void SetVertexColor( int index , RageColor c );
	void SetVertexCoords( int index , float TexCoordX , float TexCoordY );

	virtual void PushSelf( lua_State *L );

private:
	RageTexture* _Texture;

	vector<RageSpriteVertex> _Vertices;
	vector<AMV_TweenState> AMV_Tweens;
	AMV_TweenState AMV_current;
	AMV_TweenState AMV_start;

	// required to handle diffuse and glow
	AMV_TweenState *AMV_TempState;
	
	EffectMode _EffectMode;
	TextureMode _TextureMode;
};

/**
 * @file
 * @author Matthew Gardner and Eric Reese (c) 2014
 * @section LICENSE
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
